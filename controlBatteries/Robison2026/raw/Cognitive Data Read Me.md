##  Antisaccade
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *block* - participants first complete a slowed block of practice (slow_practice), then a block of practice at the regular speed (fast_practice), then a block of experimental trials which are analyzed (antisaccade)
- *trial* - a sequential identifier for the current trial
- *delay* - the duration (in milliseconds) of the "waiting" delay between the warning (***) and the cue
- *acc* - whether the participant reported the target letter correctly (1) or incorrectly (0)
- *rt* - the time between the target onset and the response (in milliseconds)
- *target_side* - whether the target letter appeared on the left or right side of the screen

## Psychomotor Vigilance Task
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *condition* - background color; gray for all participants
- *block* - for practice trials, this value is "practice", for all other trials, it is "real"
- *trial* - a sequential identifier for the current trial
- *rt* - the time between the stimulus onset and the response (in milliseconds)
- *wait_time* - the time between the stopwatch cue and stimulus onset
- *probe_resp* - if the trial ended with a thought probe, the response (if no probe, NA). responses corresponded to: 1) on-task, 2) task-related interference, 3) external distraction, 4) mind-wandering, or 5) mind-blanking

## Sustained Attention to Response Task
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *block* - for practice trials, this value is "practice", for all other trials, it is "real"
- *trial* - a sequential identifier for the current trial
- *trialtype* - "go" for any digit other than 3; "nogo" for the digit 3 to which participants should avoid responding
- *rt* - the time between the stimulus onset and the response (in milliseconds)
- *hit* - participants accurately responding to any digit other than 3
- *falsealarm* - participants mistakenly responding to the digit 3
- *probe_resp* - if the trial ended with a thought probe, the response (if no probe, NA). responses corresponded to: 1) on-task, 2) task-related interference, 3) external distraction, 4) mind-wandering, or 5) mind-blanking


## Stroop^2^
- *subID* - a unique identifier for each participant
- *session* - participants completed a round of practice trials (Practice), then a block of trials (Trials)
- *trial* - a sequential identifier for the current trial
- *seedNumber* - random generator for stimulus order
- *stimulus* - word name (red or blue) in word color (red or blue)
- *stimulusConflict* - conflict status of stimulus presentation, such that the word "red" printed in blue or the word "blue" printed in red would be conflict (1), whereas matching word color and name is no conflict (0)
- *responseConflict* - the conflict status of the correct response button, such that the word "red" printed in blue or the word "blue" printed in red would be conflict (1), whereas matching word color and name is no conflict (0)
- *response* - the appearance of the button clicked by the participant, such that the word "red" is printed in blue or the word "blue" printed is in red, or the word color and name are the same
- *buttonPosition* - position of the button that the participants clicked, 0 for left and 1 for right
- *correct* - accuracy of the participant response; 0 for incorrect and 1 for correct
- *rt* - the time between the stimulus onset and the response (in seconds)
- *dateTime* - the date and time of task completion
- *score* - the cumulative sum of accurate responses for each participant
- *computerName* - local computer where task completion occurred
- *installerName* - local user task location

## Flanker^2^
- *subID* - a unique identifier for each participant
- *session* - participants completed a round of practice trials (Practice), then a block of trials (Trials)
- *trial* - a sequential identifier for the current trial
- *seedNumber* - random generator for stimulus order
- *stimulus* - arrow direction in stimulus presentation, indicating left arrows, middle arrow, and right arrows
- *stimulusConflict* - conflict status of stimulus presentation, such that outer arrows pointing a different direction from the inner arrow is conflict (1), and arrows all point the same direction are no conflict (0)
- *responseConflict* - the conflict status of the correct response button, such that outer arrows pointing a different direction from the inner arrow is conflict (1), and arrows all point the same direction are no conflict (0)
- *response* - the appearance of the button clicked by the participant, indicating left arrows, middle arrow, and right arrows
- *buttonPosition* - position of the button that the participants clicked, 0 for left and 1 for right
- *correct* - accuracy of the participant response; 0 for incorrect and 1 for correct
- *rt* - the time between the stimulus onset and the response (in seconds)
- *dateTime* - the date and time of task completion
- *score* - the cumulative sum of accurate responses for each participant
- *computerName* - local computer where task completion occurred
- *installerName* - local user task location

## Simon^2^
- *subID* - a unique identifier for each participant
- *session* - participants completed a round of practice trials (Practice), then a block of trials (Trials)
- *trial* - a sequential identifier for the current trial
- *seedNumber* - random generator for stimulus order
- *stimulus* - arrow direction in stimulus presentation, indicating which side of the screen it appeared
- *stimulusConflict* - conflict status of stimulus presentation, such that a left-facing arrow on the right side of the screen would be conflict (1) but a left-facing arrow on the left side meant no conflict (0)
- *responseConflict* - the conflict status of the correct response button, such that the word "right" on the left side of the screen or the word "left" on the right side of the screen is conflict (1), and words that matched their side are no conflict (0)
- *response* - the appearance of the button clicked by the participant, indicating the words "left" or "right"
- *buttonPosition* - position of the button that the participants clicked, 0 for left and 1 for right
- *correct* - accuracy of the participant response; 0 for incorrect and 1 for correct
- *rt* - the time between the stimulus onset and the response (in seconds)
- *dateTime* - the date and time of task completion
- *score* - the cumulative sum of accurate responses for each participant
- *computerName* - local computer where task completion occurred
- *installerName* - local user task location

## Choice RT
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *condition* - participants completed a round of practice trials (practice), then a block of 50 trials (random)
- *block* - numerical value; trials are in 5 blocks of 10 trials
- *sub_block* - for these data, always 1
- *trial* - a sequential identifier for the current trial
- *subtrial* - an sequential identifier for the current sub-trial within a block
- *rt* - the time between the target onset and the response (in milliseconds)
- *acc* - whether the participant reported the target position correctly (1) or incorrectly (0)


## Digit Comparison
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *block* - for practice trials, this value is "0", for all other trials, it is "1" or "2"
- *trial* - a sequential identifier for the current trial
- *string* - the string of digits for the trial presented on the screen for comparison
- *match* - indicates whether the string halves are a match to one another
- *rt* - the time between stimulus/string sequences presentation and response (in milliseconds)
- *acc* - whether the participant responded correctly (1) or incorrectly (0) based on match or nonmatch
- *target_length* - the number of digits presented in each half of the string
- *total_correct* - the running total accuracy count for a participant



## Letter Comparison
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *block* - for practice trials, this value is "0", for all other trials, it is "1" or "2"
- *trial* - a sequential identifier for the current trial
- *string* - the string of letters for the trial presented on the screen for comparison
- *match* - indicates whether the string halves are a match to one another
- *rt* - the time between stimulus/string sequences presentation and response (in milliseconds)
- *acc* - whether the participant responded correctly (1) or incorrectly (0) based on match or nonmatch
- *target_length* - the number of letters presented in each half of the string
- *total_correct* - the running total accuracy count for a participant

## Operation Span
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *block* - for nonpractice trials, this says "realblock"
- *list* - a sequential identifier for the current trial that is comprised of a given set size
- *setsize* - the number of letters to remember in the list
- *matherrors* - the accuracy of the math problem presented between letters in the list, 1 = accurate, 0 = inaccurate
- *lettertotal* - the number of letters remembered accurately for the list
- *ospantotal* - the cumulative number of letters remembered for the task for each participant
- *motivation* - would be for a probe assessing motivation to complete the task; but for this study, NA
- *alertness* - would be for a probe assessing alertness to complete the task; but for this study, NA

## Symmetry Span
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *data* - date the task was completed
- *time* - time the task was completed
- *block* - for nonpractice trials, this says "realblock"
- *list* - a sequential identifier for the current trial that is comprised of a given set size
- *setsize* - the number of letters to remember in the list
- *symmetry* - the accuracy of the symmetry judgments presented between letters in the list, 1 = accurate, 0 = inaccurate
- *spantotal* - the number of letters remembered accurately for the list
- *symspantotal* - the cumulative number of letters remembered for the task for each participant

## Reading Span
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *list* - a sequential identifier for the current trial that is comprised of a given set size
- *setsize* - the number of letters to remember in the list
- *senterrors* - the accuracy of the sentence judgments presented between letters in the list, 1 = accurate, 0 = inaccurate
- *lettertotal* - the number of letters remembered accurately for the list
- *rspantotal* - the cumulative number of letters remembered for the task for each participant


## Color (change-detection)
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *block* - for practice trials, this value is "practice", for all other trials, it is 0
- *trial* - a sequential identifier for the current trial
- *trialtype* - whether the target item changed color (change) or not (nochange)
- *ss* - current set size (always 6 for these data)
- *resp* - whether the participant pressed the F key (same) or J key (different)
- *acc* - whether the participant responded correctly (1) or incorrectly (0)
- *rt* - the time between the test screen onset and the response (in milliseconds)
- *probe_resp* - if the trial ended with a thought probe, the response (if no probe, NA). responses corresponded to: 1) on-task, 2) task-related interference, 3) external distraction, 4) mind-wandering, or 5) mind-blanking




## Letter (change-detection)
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *block* - for practice trials, this value is "practice", for all other trials, it is "real"
- *trial* - a sequential identifier for the current trial
- *trialtype* - whether the target item changed color (change) or not (nochange)
- *ss* - current set size (always 6 for these data)
- *resp* - whether the participant pressed the F key (same) or J key (different)
- *acc* - whether the participant responded correctly (1) or incorrectly (0)
- *rt* - the time between the test screen onset and the response (in milliseconds)
- *probe_resp* - if the trial ended with a thought probe, the response (if no probe, NA). responses corresponded to: 1) on-task, 2) task-related interference, 3) external distraction, 4) mind-wandering, or 5) mind-blanking



## Orientation (change-detection)
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *block* - for practice trials, this value is "practice", for all other trials, it is "real"
- *screen size* - a tuple of the computer screen dimensions (in pixels)
- *trial* - a sequential identifier for the current trial
- *trialtype* - whether the target item changed color (change) or not (no-change)
- *ss* - current set size (always 6 for these data)
- *resp* - whether the participant pressed the F key (same) or J key (different)
- *acc* - whether the participant responded correctly (1) or incorrectly (0)
- *rt* - the time between the test screen onset and the response (in milliseconds)
- *probe_resp* - if the trial ended with a thought probe, the response (if no probe, NA). responses corresponded to: 1) on-task, 2) task-related interference, 3) external distraction, 4) mind-wandering, or 5) mind-blanking


## Cued Recall
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *list* - a numeric identifier for the current list (0 = practice)
- *phase* - whether the participants are in the encoding or recall phase of the task
- *serial_position* - a sequential output order for both encoding and recall
- *cue* - during encoding, cue word shown; during recall, the cue word shown on the screen
- *target* - during encoding, paired target word shown; during recall, the correct target word that should be recalled
- *response* - the word typed by the participant
- *rt* - during recall, the time between the onset of the cue word and the submission of a response

## Immediate Free Recall
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *list* - a numeric identifier for the current list (0 = practice)
- *phase* - whether the participants are in the encoding or recall phase of the task
- *serial_position* - a sequential output order for both encoding and recall
- *cue* - during encoding, cue word shown; during recall, the cue word shown on the screen; "NA" on this task
- *target* - during encoding, target word shown; during recall, the correct target word that should be recalled freely
- *rt* - during recall, the time after onset of the recall period at submission (in milliseconds)
- *correct* - accuracy of the submitted word response given its presence in the list

## Recognition
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *list* - a numeric identifier for the current list (0 = practice)
- *phase* - whether the participants are in the encoding or recall phase of the task
- *subtrial* - order of the presented words for an encoding list and recognition list
- *word* - stimulus word presented
- *old_new* - status of the word presented in the recognition list; "old" if it was present in the encoding list, and "new" if it was not in the previous list
- *acc* - accuracy of recognition; NA during the encoding phase, 1 for correct, 0 for incorrect
- *resp* - manual keyboard response; "f" for an old word and "j" for a new word
- *rt* - during the test phase, the time between the onset of the word and the submission of a response

## Raven
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *item* - item number; 18 total items
- *acc* - accuracy of response; 1 for correct, 0 for incorrect
- *rt* - the time between the stimulus onset and the response (in milliseconds)

## Number Series
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *item* - item number; 15 total items
- *acc* - accuracy of response; 1 for correct, 0 for incorrect
- *rt* - the time between the stimulus onset and the response (in milliseconds)

## Letter Sets
- *subject* - a unique identifier for each participant
- *session* - indicates whether this is the first or second attempt at the task
- *date* - the date of completion
- *time* - the time of completion
- *item* - item number; 15 total items
- *acc* - accuracy of response; 1 for correct, 0 for incorrect
- *rt* - the time between the stimulus onset and the response (in milliseconds)

## Spatial Apperception Test
- *subid* - a unique identifier for each participant
- *installerName* - local user task location
- *mode* - for practice trials, this value is "practice", for all other trials, it is "trial"
- *trialnum* - a sequential identifier for the current trial
- *correctAnswer* - multiple-choice question correct response
- *theirAnswer* - participant response to the multiple-choice questions
- *match* - accuracy of the response; 1 for correct, 0 for incorrect
- *startTime* - clock start time for the trial
- *responseTime* - the time between the stimulus onset and the response (in seconds)
- *date* - the date of completion
- *time* - the time of completion

## Terrain Orientation Task
- *SubID* - a unique identifier for each participant
- *compName* - local computer where task completion occurred
- *taskType* - indicates whether this is the first or second attempt at the task
- *Mode* - for practice trials, this value is "Training", for all other trials, it is "Experiment"
- *TrialNum* - a sequential identifier for the current trial
- *Grade* - 1 = correct, = incorrect
- *refMapName* - image name for the stimulus reference map
- *viewMapName* - image name for the stimulus rotated map
- *ImageOrientation* - degrees of rotation for the rotated map compared to the reference map
- *TheirResponse* -  degrees of rotation reported by the participant
- *TimeToRespond* - the time between the stimulus onset and the response (in seconds)
- *ISIEnd* - clock time at end of interstimulus interval
- *InitiResp* - clock time for response
- *NextClicked* - clock time for onset of item
- *trueScore* - correct minus incorrect responses
- *zeroFloorScore* - all 0s in these data
- *lastPositiveScore* - all 0s in these data
- *Difficulty* - a value (1, 2, 3) categorizing the difficulty of the item
- *itemLabel* - label for the item
- *EntryDTS* - timestamp data were entered

## Paper Folding
- *subid* - a unique identifier for each participant
- *installerName* - local user task location
- *trial* - a sequential identifier for the current trial
- *response* - participant response to the multiple-choice questions
- *answer* - multiple-choice question correct response
- *match* - accuracy of the response; 1 for correct, 0 for incorrect
- *startTime* - clock time for onset of item
- *stopTime* - clock time for offset of item (response)
- *date* - the date of completion
- *time* - timestamp of data logging