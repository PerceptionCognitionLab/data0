library(curl) ##You will need to load the R package "curl" to use this cleaning code

##Stroop Data (Data Set 1)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/FlankerStroopSimon/LEF_stroop.csv")
stroop <- read.csv2(filename, header=TRUE, dec=".")

stroop$cond <- as.numeric(as.factor(stroop$congruency))  #congruent -> 1, incongruent -> 2, neutral -> 3
ntrial <- length(stroop[stroop$ID == stroop$ID[1], 1])
nsub <- length(unique(stroop$ID))
stroop$trial <- rep(1:ntrial, nsub)
stroop$rt <- stroop$RT/1000 #rt data in seconds

stroop <- stroop[stroop$rt > .2 & stroop$rt < 2, ]
stroop <- subset(stroop, accuracy == 1 & cond != 3)


##Simon Data (Data Set 4)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/FlankerStroopSimon/LEF_simon.csv")
simon <- read.csv2(filename, header=TRUE, dec=".")

simon$cond <- as.numeric(as.factor(simon$congruency))  #congruent -> 1, incongruent -> 2, neutral -> 3
ntrial <- length(simon[simon$ID == simon$ID[1], 1])
nsub <- length(unique(simon$ID))
simon$trial <- rep(1:ntrial, nsub)
simon$rt <- simon$RT/1000

simon <- simon[simon$rt > .2 & simon$rt < 2, ]
simon <- subset(simon, accuracy == 1)

##Flanker Data (Data Set 7)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/FlankerStroopSimon/LEF_flanker.csv")
flanker <- read.csv2(filename, header=TRUE, dec=".")

flanker$cond <- as.numeric(as.factor(flanker$congruency))  #congruent -> 1, incongruent -> 2, neutral -> 3
ntrial <- length(flanker[flanker$ID == flanker$ID[1], 1])
nsub <- length(unique(flanker$ID))
flanker$trial <- rep(1:ntrial, nsub)
flanker$rt <- flanker$RT/1000

flanker <- flanker[flanker$rt > .2 & flanker$rt < 2, ]
flanker <- subset(flanker, accuracy == 1 & cond != 3)
