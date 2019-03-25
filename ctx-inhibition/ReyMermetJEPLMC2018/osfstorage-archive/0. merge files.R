### Inhibition in Aging: Merge the data files

### Select the workspace
wdpath <- paste0("~/5. Inhibition in Aging/4. Data")
# wdpath <- paste0("C:/Users/Alodie/Desktop/new/5. Inhibition in Aging/4. Data/")

### Read the participant list
setwd(paste0(wdpath))

### Merge the data files of all participants in one file

tasklist <- c("arrowflanker", "letterflanker", "colorstroop", "numberstroop", "simon",
              "global", "local", "short", "long", "n2repetitioncost",  "antisaccade", "stopsignal")

for (task in tasklist){
  # task <- "stopsignal"
  if (task == "global" || task == "local"){
    bigtask <- "globallocal"
  } else if (task == "short" || task == "long"){
    bigtask <- "negativecompatibility"
  } else {
    bigtask <- task
  }
  
  if (task == "letterflanker" || task == "numberstroop" || task == "simon"
      || task == "short" || task == "long" || task == "antisaccade"){
    form <- "form1"
  } else if (task == "arrowflanker" || task == "colorstroop" || task == "global"
             || task == "local" || task == "n2repetitioncost" || task == "stopsignal"){
    form <- "form2"
  }
  
  ## Set the workspace to read the data
  setwd(paste0(wdpath, "/data_wd/", form,"/", bigtask))
  
  file_list <- list.files()
  
  # Number of trials expected in each file
  if (task == "arrowflanker" | task == "letterflanker" 
      | task == "colorstroop" | task == "numberstroop"
      | task == "global" | task == "local" 
      | task == "short" | task == "long"){
    file_ntrials <- 320 
  } else if (task == "simon"){
    file_ntrials <- 324
  } else if (task == "n2repetitioncost"){
    file_ntrials <- 394
  } else if (task == "antisaccade"){
    file_ntrials <- 224 
  } else if (task == "stopsignal"){
    file_ntrials <- 444
  }
 
  
  for (file in file_list){
    
    temp_dataset <- read.table(file, header=F, sep="", strip.white = TRUE, comment.char="" )  
    
    # # check the number of rows in each file
    # if (nrow(temp_dataset) < file_ntrials){
    #   print(paste(file, ": file is too small!", sep=""))
    # } else if (nrow(temp_dataset) > file_ntrials){
    #   print(paste(file, ": file is too large file!", sep=""))
    # } 
    
    # if the merged dataset doesn't exist, create it
    if (!exists("mergeddata")){
     mergeddata <- temp_dataset
    } else if (exists("mergeddata")){ # if the merged dataset does exist, append to it
     mergeddata<-rbind(mergeddata, temp_dataset)
    }
    rm(temp_dataset)
  }
  
  ## Set the workspace to write the results
  setwd(paste0(wdpath))
 
  #  give name to each column of the file "mergeddata"
  if (task == "arrowflanker" | task == "letterflanker" 
      | task == "colorstroop" | task == "numberstroop"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "counterbalancing", "congruency", "stimulus", "xr", "r", "corr", "corrafter", "rt")
  } else if (task == "simon"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "congruency", "stimulus1", "stimulusposition", "xr", "r", "corr", "corrafter", "rt")
    mergeddata$stimulus <- paste0(mergeddata$stimulus1, "_", mergeddata$stimulusposition)
  } else if (task == "global"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "task", "congruency", "stimulus", "xr", "r", "corr", "corrafter", "rt")
    mergeddata <- mergeddata[which(mergeddata$task == "global-task"),]
  } else if (task == "local"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "task", "congruency", "stimulus", "xr", "r", "corr", "corrafter", "rt")
    mergeddata <- mergeddata[which(mergeddata$task == "local-task"),]
  } else if (task == "short"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "task", "fixationtiming", "blank1timing", "prime", "primetiming", "target", "targetposition", "targettiming", "congruency", "xr", "r", "corr", "corrafter", "rt")
    mergeddata <- mergeddata[which(mergeddata$task == "short"),]
    mergeddata$stimulus <- paste0(mergeddata$prime, "_", mergeddata$target)
  } else if (task == "long"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "task", "fixationtiming", "blank1timing", "prime", "primetiming", "target", "targetposition", "targettiming", "congruency", "xr", "r", "corr", "corrafter", "rt")
    mergeddata <- mergeddata[which(mergeddata$task == "long"),]
    mergeddata$stimulus <- paste0(mergeddata$prime, "_", mergeddata$target)
  } else if (task == "n2repetitioncost"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "trialtype", "cue", "congruency", "stimulus1", "stimulus2", "stimulus3", "stimulus4", "xr", "r", "corr", "corrafter", "rt")
    mergeddata$stimulus <- paste0(mergeddata$stimulus1, "_", mergeddata$stimulus2, "_", mergeddata$stimulus3, "_", mergeddata$stimulus4)
  } else if (task == "antisaccade"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "congruency", "block", "trialtype", "fixationtiming", "flashposition", "flashtiming", "target", "stimulusposition", "stimulusduration", "nextstimulusduration", "stimulustiming", "masktiming", "xr", "r", "corr", "corrafter", "rt")
    mergeddata$stimulus <- paste0(mergeddata$flashposition, "_", mergeddata$target, "_", mergeddata$stimulusposition)
  } else if (task == "stopsignal"){
    colnames(mergeddata) <- c("subj","age", "age_group", "experimenter",  "weekday", "day", "month", "year", "time", "counterbalancing_battery", "block", "blocktype", "trialtype", "congruency", "stimulus", "stimulus2", "xr", "r1", "r2", "rt1", "rt2", "ssd", "ssdnext", "corr", "corrafter", "rt")
  }
  
  ## convert the variable in factor 
  mergeddata$age_group<-factor(mergeddata$age_group, levels=c(1,3), labels=c("young", "old"))
  mergeddata$counterbalancing_battery<-as.factor(mergeddata$counterbalancing_battery)
  mergeddata$counterbalancing <- ifelse(mergeddata$counterbalancing == "c1", "c1",
                                         ifelse(mergeddata$counterbalancing == "c2", "c2",
                                                ifelse(mergeddata$counterbalancing == "c3", "c3",NA)))
  
  ## seconds to milliseconds
  mergeddata$rt <- mergeddata$rt*1000
  
  ## Rectify the counterbalancing of some participants
  mergeddata$counterbalancing_battery[which(mergeddata$subj == 166)] <- 2
  mergeddata$counterbalancing_battery[which(mergeddata$subj == 222)] <- 1
  mergeddata$counterbalancing_battery[which(mergeddata$subj == 334)] <- 2
  mergeddata$counterbalancing_battery[which(mergeddata$subj == 340)] <- 1
  mergeddata$counterbalancing_battery[which(mergeddata$subj == 418)] <- 2
  
  ## select only the relevant blocks and trials 
  if (task == "arrowflanker" | task == "letterflanker" 
    | task == "colorstroop" | task == "numberstroop"
    | task == "n2repetitioncost"){
      mergeddata <- mergeddata[which(mergeddata$block!= "practice"   
                                 & mergeddata$trialtype == "exp"),]
      mergeddata$trial <- 1:72
      if (task == "n2repetitioncost"){
        ntrials <- 360
      } else {
        ntrials <- 288
      }
  } else if (task == "simon"){
    mergeddata <- mergeddata[which((mergeddata$block == "block1" 
                              | mergeddata$block == "block2"
                              | mergeddata$block == "block3" 
                              | mergeddata$block == "block4"
                              | mergeddata$block == "block5" 
                              | mergeddata$block == "block6")   
                             & mergeddata$trialtype == "exp"),]
    mergeddata$trial <- 1:48
    ntrials <- 288
  } else if (task == "global"){
    mergeddata <- mergeddata[which((mergeddata$block == "block1" 
                              | mergeddata$block == "block4")
                             & mergeddata$trialtype == "exp"),]
    mergeddata$trial <- 1:72
    ntrials <- 144
  } else if (task == "local"){
    mergeddata <- mergeddata[which((mergeddata$block == "block2" 
                              | mergeddata$block == "block3")
                             & mergeddata$trialtype == "exp"),]
    mergeddata$trial <- 1:72
    ntrials <- 144
  } else if (task == "short"){
    mergeddata <- mergeddata[which((mergeddata$block == "block1" 
                              | mergeddata$block == "block4")
                             & mergeddata$trialtype == "exp"),]
    mergeddata$trial <- 1:72
    ntrials <- 144
  } else if (task == "long"){
    mergeddata <- mergeddata[which((mergeddata$block == "block2" 
                              | mergeddata$block == "block3")
                             & mergeddata$trialtype == "exp"),]
    mergeddata$trial <- 1:72
    ntrials <- 144
  } else if (task == "antisaccade"){
    mergeddata <- mergeddata[which(mergeddata$block!= "practice"   
                                   & mergeddata$trialtype == "exp"),]
   
    mergeddata$blocktype <- ""
    mergeddata$blocktype[mergeddata$block=="block1" & mergeddata$congruency=="prosaccade"] <- "block1"
    mergeddata$blocktype[mergeddata$block=="block2" & mergeddata$congruency=="prosaccade"] <- "block2"
    mergeddata$blocktype[mergeddata$block=="block1" & mergeddata$congruency=="antisaccade"] <- "block3"
    mergeddata$blocktype[mergeddata$block=="block2" & mergeddata$congruency=="antisaccade"] <- "block4"
    mergeddata$trial <- 0
    mergeddata$trial[mergeddata$congruency == "prosaccade"] <- 1:40
    mergeddata$trial[mergeddata$congruency == "antisaccade"] <- 1:48
    mergeddata$block <- mergeddata$blocktype
    mergeddata$blocktype <- NULL
    ntrials <- 176
  } else if (task == "stopsignal"){
    mergeddata$ssd <- mergeddata$ssd*1000
    mergeddata$congruency <- 
      ifelse((mergeddata$congruency == "go" & mergeddata$blocktype == "onlygo"), "onlygo", 
             ifelse((mergeddata$congruency == "go" & mergeddata$blocktype == "gonogo"), "go", 
                    ifelse((mergeddata$congruency == "nogo" & mergeddata$blocktype == "gonogo"), "nogo", "false")))
    mergeddata <- mergeddata[which((mergeddata$block == "block2" 
                                    | mergeddata$block == "block3"                           
                                    | mergeddata$block == "block4" 
                                    | mergeddata$block == "block5"
                                    | mergeddata$block == "block1"
                                    | mergeddata$block == "block6")
                                   & mergeddata$trialtype == "exp"),] 
    mergeddata$trial <- 0
    mergeddata$trial[mergeddata$block == "block1" | mergeddata$block == "block6"] <- 1:48
    mergeddata$trial[mergeddata$block == "block2" | mergeddata$block == "block3"
                     | mergeddata$block == "block4" | mergeddata$block == "block5"] <- 1:72
    ntrials <- 384
  }
  
  ## Only include the relevant participants (i.e., those from the study Inhibition in Aging)
  #dataset <- merge(list, mergeddata)
  dataset <- mergeddata
  
  ## Re-order the the file according to subj, block and trial
  dataset <- dataset[order(dataset[,colnames(dataset)=="subj"],
                           dataset[,colnames(dataset)=="block"], 
                           dataset[,colnames(dataset)=="trial"]),]
  
  ## Remove irrelevant columns
  if (task == "stopsignal"){
    dat <- dataset[, colnames(dataset) %in% c("subj", "age", "age_group", "counterbalancing_battery",
                                              "counterbalancing", "block", "stimulus", "trial", "congruency",
                                              "corr", "rt", "ssd")]
    dat$ssd <- ifelse(dat$congruency == "nogo", dat$ssd, NA)
  } else {
    dat <- dataset[, colnames(dataset) %in% c("subj", "age", "age_group", "counterbalancing_battery",
                                              "counterbalancing", "block", "stimulus", "trial", "congruency",
                                              "corr", "rt")]
    dat$ssd <- NA
  }
  
  dat$task <- task # add the name of the task
  
  dat <- dat[,c("subj", "age", "age_group", "counterbalancing_battery", "counterbalancing", "task", "block", "stimulus", "trial", "ssd", "congruency", "corr", "rt")] # reorder the columns
 
  ## check whether each participant has a complete dataset for each task
  for (i in unique(dat$subj)){
    temp <- dat[which(dat$subj == i),]
    if (nrow(temp) < ntrials){
      print(paste0(task, " - ", i, ": ntrials is too small!"))
    } else if (nrow(temp) > ntrials){
      print(paste0(task, " - ", i, ": ntrials is too large!"))
    }
    rm(temp)
  }
  
 
  ## Merge the data of all tasks together
  if (!exists("d")){
    d <- dat
  } else if (exists("d")){
    d <- rbind(d, dat)
  }
  
  rm(mergeddata, dataset, dat)
}

d$corr <- ifelse(d$corr > 2, 99, d$corr ) # codify similarly irrelevant rows

## remove the excluded participants
d_rp <- d[which(d$subj != 112 # high BDI
              & d$subj != 132 # same tasks in both sessions (experimenter error)
              & d$subj != 139 # high BDI
              & d$subj != 161 # high BDI
              & d$subj != 164 # same tasks in both sessions (experimenter error)
              & d$subj != 184 # low MMS  
              & d$subj != 196 # high BDI
              & d$subj != 198 # high BDI
              & d$subj != 225 # high BDI
              & d$subj != 227 # high BDI
              & d$subj != 301 # low MMS  
              & d$subj != 303 # high GDS
              & d$subj != 309 # low MMS  
              & d$subj != 311 # high GDS
              & d$subj != 313 # low MMS  
              & d$subj != 326 # age 64 (too young)
              & d$subj != 363 # age 76 (too old)
              & d$subj != 365 # age 63 (too young)
              & d$subj != 368 # Participant did not come in the second session
              & d$subj != 373 # low MMS  
              & d$subj != 374 # age 76 (too old)
              & d$subj != 402 # Participant did not come in the second session  
              & d$subj != 423 # low MMS
              & d$subj != 426 # low MMS
              & d$subj != 429 # Participant did not come in the second session  
              & d$subj != 444),] # high GDS & BDI


## Compute the mean accuracy for each participant in each block of each task
check <-aggregate(d_rp$corr, by=list(d_rp$subj,
                                     d_rp$task,
                                     d_rp$block),
                    FUN=mean, na.rm=TRUE)
colnames(check) <- c("subj", "task", "block", "acc")


## Prepare to remove participants with low accuracy (i.e., less than 75% for tasks with 2 choices, and 50% for tasks with 4 choices), antisaccade was removed because the dependent measure is on accuracy
check2 <- check[which(check$task == "arrowflanker" |  check$task == "letterflanker"
                      | check$task == "simon" | check$task == "stopsignal" 
                      |  check$task == "global" | check$task == "local" 
                      |  check$task == "short"  | check$task == "long"),]
ch2 <- check2[which(check2$acc < 0.75),]

check4 <- check[which(check$task == "colorstroop" |  check$task == "numberstroop"
                      | check$task == "n2repetitioncost"),]
ch4 <- check4[which(check4$acc < 0.50),]

ch <- merge(ch2, ch4, all = TRUE)
ch$crit <- "block"
# check whether the complete task must be removed (i.e., when two blocks or more show low accuracy)
for (i in unique(ch$subj)){
  temp <- ch[which(ch$subj == i),]
  for (j in unique(temp$task)){
    temptemp <- temp[which(temp$task == j),]
    if(nrow(temptemp) > 1){
      ch$crit[which(ch$subj == i & ch$task == j)] <- "task"
    }
    rm(temptemp)
  }
  rm(temp)
}
ch_block <- ch[which(ch$crit == "block"),]
ch_block$condition <- paste0(ch_block$subj, "_", ch_block$task, "_", ch_block$block)
ch_task <- ch[which(ch$crit == "task"),]
ch_task$condition <- paste0(ch_task$subj, "_", ch_task$task)

d_rp$condition_block <- paste0(d_rp$subj, "_", d_rp$task, "_", d_rp$block)
d_rp$condition_task <- paste0(d_rp$subj, "_", d_rp$task)

d_rp$sel_block <- !d_rp$condition_block %in% ch_block$condition
d_rp$sel_task <- !d_rp$condition_task %in% ch_task$condition

# Set the variable indicating the inclusion (yes) or the exclusion (no) of the trial according to the accuracy criterion
d_rp$crit_val <- ifelse(d_rp$sel_block == "FALSE" | d_rp$sel_task == "FALSE", "no", "yes")
d_rp <- d_rp[,c(1:5, ncol(d_rp), 6:13)]
rm(ch, ch_block, ch_task, ch2, ch4, check2, check4)

## Preparte to remove participants only if their respective mean for each block of each task is below 2.5 SD
check <- check[which(check$task != "antisaccade"),]  ## remove antisaccade because the dependent measure is on accuracy
check$crit <- NA
for (i in unique(check$subj)){
  for (j in unique(check$task)){
    for (k in unique(check$block)){
     temp <- check[which(check$subj == i & check$task == j & check$block == k),] 
     temp_all <- check[which(check$task == j & check$block == k),] 
     check$crit[which(check$subj == i & check$task == j & check$block == k)] <- temp$acc > mean(temp_all$acc) - 2.5*sd(temp_all$acc)
     rm(temp, temp_all)
    }
  }
}
check$crit <- ifelse(check$crit == "FALSE", "block", "yes")
# check whether the complete task must be removed (i.e., when two blocks or more show low accuracy)
for (i in unique(check$subj)){
  temp <- check[which(check$subj == i & check$crit == "block"),]
  for (j in unique(temp$task)){
    temptemp <- temp[which(temp$task == j),]
    if(nrow(temptemp) > 1){
      check$crit[which(check$subj == i & check$task == j & check$crit == "block")] <- "task"
    }
    rm(temptemp)
  }
  rm(temp)
}
ch <- check[which(check$crit != "yes"),]

ch_block <- ch[which(ch$crit == "block"),]
ch_block$condition <- paste0(ch_block$subj, "_", ch_block$task, "_", ch_block$block)
ch_task <- ch[which(ch$crit == "task"),]
ch_task$condition <- paste0(ch_task$subj, "_", ch_task$task)

d_rp$condition_block <- paste0(d_rp$subj, "_", d_rp$task, "_", d_rp$block)
d_rp$condition_task <- paste0(d_rp$subj, "_", d_rp$task)

d_rp$sel_block <- !d_rp$condition_block %in% ch_block$condition
d_rp$sel_task <- !d_rp$condition_task %in% ch_task$condition

# Set the variable indicating the inclusion (yes) or the exclusion (no) of the trial according to the accuracy criterion if accuracy was smaller than 2.5 SD
d_rp$crit_sd <- ifelse(d_rp$sel_block == "FALSE" | d_rp$sel_task == "FALSE", "no", "yes")

# Set the variable indicating the inclusion of all participants
d_rp$crit_all <- "yes" 
d_rp <- d_rp[,c(1:5, ncol(d_rp), 6, (ncol(d_rp)-1), 7:14)]
rm(check, ch, ch_block, ch_task)

## Prepare to remove participants according to visual screening
list_subj <- c(101, 	102, 	103, 	104, 	105, 	106, 	107, 	108, 	109, 	110, 	111, 	113, 	114, 	115, 	116, 	117, 	118, 	119, 	120, 	121, 	122, 	123, 	124, 	125, 	126, 	127, 	128, 	129, 	130, 	134, 	135, 	137, 	138, 	142, 	143, 	144, 	145, 	148, 	149, 	150, 	151, 	152, 	153, 	154, 	155, 	156, 	157, 	158, 	159, 	160, 	162, 	163, 	165, 	166, 	167, 	168, 	169, 	170, 	172, 	173, 	174, 	175, 	176, 	177, 	178, 	179, 	180, 	182, 	185, 	186, 	187, 	188, 	189, 	190, 	191, 	192, 	193, 	194, 	195, 	197, 	199, 	200, 	201, 	202, 	203, 	204, 	205, 	206, 	207, 	208, 	209, 	210, 	211, 	212, 	213, 	214, 	215, 	216, 	217, 	218, 	219, 	220, 	221, 	222, 	223, 	224, 	226, 	228, 	229, 	230, 	302, 	305, 	307, 	310, 	312, 	314, 	315, 	316, 	320, 	321, 	322, 	323, 	325, 	327, 	329, 	330, 	331, 	333, 	334, 	335, 	337, 	338, 	339, 	340, 	341, 	343, 	345, 	346, 	347, 	348, 	349, 	350, 	351, 	352, 	353, 	356, 	357, 	358, 	360, 	361, 	362, 	364, 	366, 	367, 	370, 	371, 	372, 	375, 	376, 	377, 	378, 	379, 	381, 	382, 	383, 	385, 	386, 	387, 	388, 	390, 	391, 	392, 	393, 	394, 	396, 	398, 	399, 	401, 	403, 	404, 	405, 	406, 	407, 	408, 	412, 	413, 	414, 	415, 	416, 	417, 	418, 	419, 	421, 	422, 	424, 	425, 	427, 	428, 	432, 	434, 	436, 	437, 	438, 	439, 	440, 	441, 	443, 	445, 	446, 	447, 	448, 	449, 	450, 	451, 	452, 	455, 	456, 	458, 	459, 	460)

d_rp$sel <- d_rp$subj %in% list_subj

# Set the variable indicating the inclusion (yes) or the exclusion (no) of the trial according to visual screening (vs)
d_rp$crit_vs <- ifelse(d_rp$sel == "FALSE", "no", "yes")
d_rp <- d_rp[,c(1:6, ncol(d_rp), 7:16)]

## Write the results
library(MASS)
dir.create("datasets", showWarnings = FALSE)
setwd(paste0(wdpath, "/datasets/"))
write.matrix(d_rp,file=paste0("mergeddata.txt"), sep = "\t" )

# rm(list = ls())