# compute_aggregate_scores.R
#
# Computes aggregate scores for all 24 tasks × 2 sessions from the Robison et al. (2026)
# raw data files, using the correct scoring methods identified in the discrepancy report.
#
# Key corrections vs. ac_both_sessions_z.csv:
#   - Stroop², Flanker², Simon²: CMI (correct minus incorrect) not number-correct
#   - IFR: deduplicate recall entries before counting correct
#   - All measures: multipass ±3 SD outlier exclusion
#
# Output: wide-format dataframe with z-scored columns, mirroring ac_both_sessions_z.csv

library(dplyr)
library(tidyr)
library(readr)

DATA_DIR <- "_robison-data"

# ---------------------------------------------------------------------------
# Helper: multipass ±3 SD outlier exclusion
# Iteratively removes subjects > 3 SD from mean until no more are removed.
# Returns a logical vector (TRUE = keep).
# ---------------------------------------------------------------------------
multipass_outlier <- function(x) {
  keep <- !is.na(x)
  if (sum(keep) == 0) return(keep)
  repeat {
    m  <- mean(x[keep], na.rm = TRUE)
    s  <- sd(x[keep],   na.rm = TRUE)
    if (is.na(s) || s == 0) break
    new_keep <- keep & (is.na(x) | (x >= m - 3 * s & x <= m + 3 * s))
    if (all(new_keep == keep)) break
    keep <- new_keep
  }
  keep
}

# Apply outlier exclusion: sets out-of-range values to NA, returns updated vector
apply_outlier <- function(x) {
  keep <- multipass_outlier(x)
  x[!keep] <- NA
  x
}

# ---------------------------------------------------------------------------
# Helper: z-score a vector (using only non-NA values for mean/sd)
# ---------------------------------------------------------------------------
zscore <- function(x) {
  m <- mean(x, na.rm = TRUE)
  s <- sd(x,   na.rm = TRUE)
  (x - m) / s
}

# ---------------------------------------------------------------------------
# Helper: read a CSV from the data directory
# ---------------------------------------------------------------------------
read_data <- function(file) {
  read_csv(file.path(DATA_DIR, file), show_col_types = FALSE)
}

# ---------------------------------------------------------------------------
# Score one session for a given task.
# Returns a data.frame with columns: subject, score
# ---------------------------------------------------------------------------

# --- Antisaccade ---
score_antisaccade <- function(session) {
  df <- read_data(sprintf("antisaccade_session%d.csv", session))
  df %>%
    filter(block == "antisaccade") %>%
    group_by(subject) %>%
    summarise(score = mean(acc, na.rm = TRUE), .groups = "drop")
}

# --- PVT ---
# DV: mean of the slowest quintile (RTs at/above 80th percentile)
# Exclusions: RTs < 200 ms or > 3000 ms
score_pvt <- function(session) {
  df <- read_data(sprintf("pvt_session%d.csv", session))
  df %>%
    filter(block == "real", rt >= 0.200, rt <= 3.000) %>%
    group_by(subject) %>%
    summarise(
      score = {
        rts  <- rt
        cutoff <- quantile(rts, 0.80, na.rm = TRUE)
        mean(rts[rts >= cutoff], na.rm = TRUE)
      },
      .groups = "drop"
    )
}

# --- SART ---
# DV: SD of RT on go trials (real block)
# Exclusion: subjects with < 50% go-trial response rate
score_sart <- function(session) {
  df <- read_data(sprintf("sart_session%d.csv", session))
  real <- df %>% filter(block == "real")

  go_trials <- real %>% filter(trialtype == "go")

  # Response rate per subject on go trials (non-NA RT = responded)
  resp_rate <- go_trials %>%
    group_by(subject) %>%
    summarise(rate = mean(!is.na(rt)), .groups = "drop")

  go_sd <- go_trials %>%
    group_by(subject) %>%
    summarise(score = sd(rt, na.rm = TRUE), .groups = "drop")

  go_sd %>%
    left_join(resp_rate, by = "subject") %>%
    mutate(score = if_else(rate < 0.50, NA_real_, score)) %>%
    select(subject, score)
}

# --- Stroop², Flanker², Simon² ---
# DV: correct minus incorrect responses (CMI) in the 90-s timed period
# The `score` column in the raw file is the running CMI total;
# the final value per subject is the CMI score.
score_cmi_task <- function(task, session) {
  df <- read_data(sprintf("%s_session%d.csv", task, session))
  df %>%
    filter(session_block == "Trials") %>%   # experimental (non-practice) block
    group_by(subID) %>%
    slice_tail(n = 1) %>%
    ungroup() %>%
    transmute(subject = subID, score = score)
}

# Wrapper: detect the block column name (session vs session_block) and filter
score_stroop_flanker_simon <- function(task, session) {
  df <- read_data(sprintf("%s_session%d.csv", task, session))
  # Identify the block column (may be 'session' or 'session_block')
  block_col <- intersect(names(df), c("session_block", "session"))[1]
  subid_col <- intersect(names(df), c("subID", "subject"))[1]
  df %>%
    filter(.data[[block_col]] == "Trials") %>%
    group_by(across(all_of(subid_col))) %>%
    slice_tail(n = 1) %>%
    ungroup() %>%
    transmute(subject = as.numeric(.data[[subid_col]]), score = score)
}

# --- Letter Comparison & Digit Comparison ---
# DV: total_correct (last row per subject across real blocks 1 and 2)
score_comparison <- function(task, session) {
  df <- read_data(sprintf("%s_session%d.csv", task, session))
  df %>%
    filter(block %in% c(1, 2, "1", "2")) %>%
    group_by(subject) %>%
    slice_tail(n = 1) %>%
    ungroup() %>%
    transmute(subject = as.numeric(subject), score = total_correct)
}

# --- Choice RT ---
# DV: median RT on accurate trials (real block)
# Trial exclusions: RT < 200 ms or > 5000 ms
# Subject exclusion: < 75% accuracy
score_choicert <- function(session) {
  df <- read_data(sprintf("choicert_session%d.csv", session))
  # block == 0 is practice in some files; keep non-zero blocks
  real <- df %>% filter(block != 0, block != "practice", block != "0")

  # Trial-level exclusions
  real <- real %>% filter(rt >= 200, rt <= 5000)

  # Subject-level accuracy
  acc_rate <- real %>%
    group_by(subject) %>%
    summarise(acc_rate = mean(acc, na.rm = TRUE), .groups = "drop")

  med_rt <- real %>%
    filter(acc == 1) %>%
    group_by(subject) %>%
    summarise(score = median(rt, na.rm = TRUE), .groups = "drop")

  med_rt %>%
    left_join(acc_rate, by = "subject") %>%
    mutate(score = if_else(acc_rate < 0.75, NA_real_, score)) %>%
    select(subject, score)
}

# --- Operation Span ---
score_ospan <- function(session) {
  df <- read_data(sprintf("ospan_session%d.csv", session))
  df %>%
    filter(block == "realblock") %>%
    group_by(subject) %>%
    slice_tail(n = 1) %>%
    ungroup() %>%
    transmute(subject = as.numeric(subject), score = ospantotal)
}

# --- Symmetry Span ---
score_symspan <- function(session) {
  df <- read_data(sprintf("symspan_session%d.csv", session))
  df %>%
    filter(block == "realblock") %>%
    group_by(subject) %>%
    slice_tail(n = 1) %>%
    ungroup() %>%
    transmute(subject = as.numeric(subject), score = symspantotal)
}

# --- Reading Span ---
# No block column; all rows are real trials
score_rspan <- function(session) {
  df <- read_data(sprintf("rspan_session%d.csv", session))
  df %>%
    group_by(subject) %>%
    slice_tail(n = 1) %>%
    ungroup() %>%
    transmute(subject = as.numeric(subject), score = rspantotal)
}

# --- Change Detection (Color, Orientation, Letter) ---
# DV: mean accuracy on real (non-practice) trials
# Color: real block is labelled "0" (not "practice")
# Orientation & Letter: real block is labelled "real"
score_change_detection <- function(task, session) {
  df <- read_data(sprintf("%s_session%d.csv", task, session))
  real_vals <- if (task == "color") c("0") else c("real")
  df %>%
    filter(block %in% real_vals) %>%
    group_by(subject) %>%
    summarise(score = mean(acc, na.rm = TRUE), .groups = "drop") %>%
    mutate(subject = as.numeric(subject))
}

# --- IFR (Immediate Free Recall) ---
# DV: mean correct per list, after deduplicating repeated recall entries
# Deduplication: take max(correct) per (subject, list, target)
score_ifr <- function(session) {
  df <- read_data(sprintf("ifr_session%d.csv", session))
  df %>%
    filter(phase == "recall" | !is.na(target)) %>%
    group_by(subject, list, target) %>%
    summarise(correct = max(correct, na.rm = TRUE), .groups = "drop") %>%
    group_by(subject, list) %>%
    summarise(correct_per_list = sum(correct, na.rm = TRUE), .groups = "drop") %>%
    group_by(subject) %>%
    summarise(score = mean(correct_per_list, na.rm = TRUE), .groups = "drop") %>%
    mutate(subject = as.numeric(subject))
}

# --- Cued Recall ---
# DV: proportion correct out of non-timed-out (non-NA response) trials
score_cued_recall <- function(session) {
  df <- read_data(sprintf("cued_recall_session%d.csv", session))
  df %>%
    filter(phase == "recall") %>%
    mutate(
      correct = if_else(
        !is.na(response),
        as.numeric(trimws(tolower(response)) == trimws(tolower(target))),
        NA_real_
      )
    ) %>%
    group_by(subject) %>%
    summarise(
      score = sum(correct, na.rm = TRUE) / sum(!is.na(response)),
      .groups = "drop"
    ) %>%
    mutate(subject = as.numeric(subject))
}

# --- Recognition Memory ---
# DV: mean accuracy on test phase, list > 0
score_recognition <- function(session) {
  df <- read_data(sprintf("recognition_session%d.csv", session))
  df %>%
    filter(phase == "test", list > 0) %>%
    group_by(subject) %>%
    summarise(score = mean(acc, na.rm = TRUE), .groups = "drop") %>%
    mutate(subject = as.numeric(subject))
}

# --- Raven's Progressive Matrices ---
score_raven <- function(session) {
  df <- read_data(sprintf("raven_session%d.csv", session))
  df %>%
    group_by(subject) %>%
    summarise(score = sum(acc, na.rm = TRUE), .groups = "drop") %>%
    mutate(subject = as.numeric(subject))
}

# --- Number Series ---
score_number_series <- function(session) {
  df <- read_data(sprintf("ns_session%d.csv", session))
  df %>%
    group_by(subject) %>%
    summarise(score = sum(acc, na.rm = TRUE), .groups = "drop") %>%
    mutate(subject = as.numeric(subject))
}

# --- Letter Sets ---
score_letter_sets <- function(session) {
  df <- read_data(sprintf("ls_session%d.csv", session))
  df %>%
    group_by(subject) %>%
    summarise(score = sum(acc, na.rm = TRUE), .groups = "drop") %>%
    mutate(subject = as.numeric(subject))
}

# --- Paper Folding ---
# DV: sum of match on installerName == 'UTA Session 2' numeric trials (all trials incl. trial 1)
score_paper_folding <- function(session) {
  df <- read_data(sprintf("paper_folding_session%d.csv", session))
  # installerName varies by session: "UTA Session 2" (session 1), "UTA Session 4" (session 2)
  # Both share the pattern "UTA Session *" and contain numeric trial IDs
  df %>%
    filter(grepl("^UTA Session", installerName),
           suppressWarnings(!is.na(as.numeric(as.character(trial))))) %>%
    group_by(subid) %>%
    summarise(score = sum(match, na.rm = TRUE), .groups = "drop") %>%
    transmute(subject = as.numeric(subid), score)
}

# --- SAT (Speed-Accuracy Tradeoff) ---
# DV: correct minus incorrect on mode == 'trial' rows
score_sat <- function(session) {
  df <- read_data(sprintf("sat_session%d.csv", session))
  df %>%
    filter(mode == "trial") %>%
    group_by(subid) %>%
    summarise(
      score = sum(match == 1, na.rm = TRUE) - sum(match == 0, na.rm = TRUE),
      .groups = "drop"
    ) %>%
    transmute(subject = as.numeric(subid), score)
}

# --- TOT (Tower of Thought / Trail-making variant) ---
# DV: sum of Grade on Mode == 'Experiment' rows
score_tot <- function(session) {
  df <- read_data(sprintf("tot_session%d.csv", session))
  df %>%
    filter(Mode == "Experiment") %>%
    group_by(SubID) %>%
    summarise(score = sum(Grade, na.rm = TRUE), .groups = "drop") %>%
    transmute(subject = as.numeric(SubID), score)
}

# ---------------------------------------------------------------------------
# Main: compute all scores, apply outlier exclusion, z-score, merge to wide
# ---------------------------------------------------------------------------

# Map of column name → scoring function call
task_list <- list(
  antisaccade   = function(s) score_antisaccade(s),
  pvt_slow20    = function(s) score_pvt(s),
  sart_rt_sd    = function(s) score_sart(s),
  stroop        = function(s) score_stroop_flanker_simon("stroop", s),
  simon         = function(s) score_stroop_flanker_simon("simon", s),
  flanker       = function(s) score_stroop_flanker_simon("flanker", s),
  letter_comp   = function(s) score_comparison("lettercomp", s),
  digit_comp    = function(s) score_comparison("digitcomp", s),
  choice_rt     = function(s) score_choicert(s),
  ospan         = function(s) score_ospan(s),
  symspan       = function(s) score_symspan(s),
  rspan         = function(s) score_rspan(s),
  color         = function(s) score_change_detection("color", s),
  orientation   = function(s) score_change_detection("orientation", s),
  letter        = function(s) score_change_detection("letter", s),
  ifr           = function(s) score_ifr(s),
  cued_recall   = function(s) score_cued_recall(s),
  recognition   = function(s) score_recognition(s),
  raven         = function(s) score_raven(s),
  number_series = function(s) score_number_series(s),
  letter_sets   = function(s) score_letter_sets(s),
  paper_folding = function(s) score_paper_folding(s),
  sat           = function(s) score_sat(s),
  tot           = function(s) score_tot(s)
)

# Collect all subjects from the aggregate file (gold standard subject list)
all_subjects <- read_data("ac_both_sessions_z.csv") %>%
  mutate(subject = as.numeric(subject)) %>%
  select(subject)

# Build wide dataframe
result <- all_subjects

for (task_name in names(task_list)) {
  for (session in 1:2) {
    col_name <- sprintf("%s_%d", task_name, session)
    message(sprintf("Computing %s ...", col_name))

    scores_raw <- tryCatch(
      task_list[[task_name]](session),
      error = function(e) {
        warning(sprintf("Error in %s: %s", col_name, e$message))
        NULL
      }
    )

    if (is.null(scores_raw)) next

    # Rename to standard subject column
    scores_raw <- scores_raw %>%
      mutate(subject = as.numeric(subject)) %>%
      select(subject, score)

    # Apply outlier exclusion (sets extreme values to NA)
    scores_raw <- scores_raw %>%
      mutate(score = {
        v <- score
        v[!multipass_outlier(v)] <- NA
        v
      })

    # Z-score
    scores_raw <- scores_raw %>%
      mutate(score = zscore(score))

    # Rename score column
    names(scores_raw)[2] <- col_name

    result <- result %>%
      left_join(scores_raw, by = "subject")
  }
}

# ---------------------------------------------------------------------------
# Write output
# ---------------------------------------------------------------------------
write_csv(result, "aggregate_scores_corrected.csv")
message("\nDone. Written to aggregate_scores_corrected.csv")
message(sprintf("Dimensions: %d rows × %d columns", nrow(result), ncol(result)))

# ---------------------------------------------------------------------------
# Validation: compare with original aggregate on key correlations
# ---------------------------------------------------------------------------
message("\n--- Validation: spot-check correlations vs. ac_both_sessions_z.csv ---")

original <- read_data("ac_both_sessions_z.csv") %>%
  mutate(subject = as.numeric(subject))

check_pairs <- list(
  c("stroop_1", "simon_1"),
  c("stroop_1", "sart_rt_sd_1"),
  c("flanker_1", "antisaccade_1"),
  c("antisaccade_1", "pvt_slow20_1"),
  c("sart_rt_sd_1", "pvt_slow20_1")
)

ref_labels <- c(
  "stroop vs simon (Table 4 paper: +0.34)",
  "stroop vs sart (Table 4 paper: -0.47)",
  "flanker vs antisaccade (Table 4 paper: +0.45)",
  "antisaccade vs pvt (Table 4 paper: -0.41)",
  "sart vs pvt (Table 4 paper: +0.41)"
)

for (i in seq_along(check_pairs)) {
  cols  <- check_pairs[[i]]
  label <- ref_labels[[i]]

  # Original aggregate correlation
  orig_sub <- original %>% select(subject, all_of(cols)) %>% drop_na()
  r_orig <- if (nrow(orig_sub) > 2)
    cor(orig_sub[[cols[1]]], orig_sub[[cols[2]]]) else NA

  # Corrected correlation
  corr_sub <- result %>% select(subject, all_of(cols)) %>% drop_na()
  r_corr <- if (nrow(corr_sub) > 2)
    cor(corr_sub[[cols[1]]], corr_sub[[cols[2]]]) else NA

  message(sprintf(
    "  %s\n    Original: %.3f  |  Corrected: %.3f",
    label, r_orig, r_corr
  ))
}



