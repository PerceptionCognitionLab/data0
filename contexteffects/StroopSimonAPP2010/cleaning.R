library(curl) ##You will need to load the R package "curl" to use this cleaning code

##Experiment 1, Stroop task (Data Set 2)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/StroopSimonAPP2010/allsi2.dat")
clnames <- c('exp'
             , 'sub'
             , 'blk'
             , 'trial'
             , 'color'
             , 'distract'
             , 'cond'
             , 'resp'
             , 'acc'
             , 'rt'
             , 'errorTotal')

dat <- read.table(filename)
colnames(dat) <- clnames

#clean rt data as proposed in Pratte et al., 2010
dat <- dat[dat$rt > .2 & dat$rt < 2, ] #Delete very slow and very fast responses
dat <- subset(dat, acc == 1 & cond != 2 & exp == 1) #accurate data, deleting neutral condition, only stroop task data
dat <- dat[!(dat$trial %in% 1:5), ] #Delete first 5 trials in each block

dat.stroop.p1 <- dat


##Experiment 2, Stroop task (Data Set 3)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/StroopSimonAPP2010/allsi7.dat")
clnames <- c('sub'
             ,'blk'
             ,'blktype'
             ,'trial'
             ,'word'
             ,'location'
             ,'cond'
             ,'resp'
             ,'acc'
             ,'rt'
             ,'errorTotal')

dat <- read.table(filename)
colnames(dat) <- clnames

#clean rt data as proposed in Pratte et al., 2010
#only congruent & incongruent condition and only stroop data
dat <- dat[dat$rt > .2 & dat$rt < 2, ]
dat <- subset(dat, acc == 1 & blktype == 1) #Only accurate data, only data from Stroop task
dat <- dat[!(dat$trial %in% 1:5), ]

dat.stroop.p2 <- dat


##Experiment 1, Simon task (Data Set 5)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/StroopSimonAPP2010/allsi2.dat")
clnames <- c('exp'
             , 'sub'
             , 'blk'
             , 'trial'
             , 'color'
             , 'distract'
             , 'cond'
             , 'resp'
             , 'acc'
             , 'rt'
             , 'errorTotal')

dat <- read.table(filename)
colnames(dat) <- clnames

#clean rt data as proposed in Pratte et al., 2010
#only congruent & incongruent condition and only stroop data
dat <- dat[dat$rt > .2 & dat$rt < 2, ]
dat <- subset(dat, acc == 1 & exp == 0) #accurate data, only simon task data
dat <- dat[!(dat$trial %in% 1:5), ]

dat.simon.p1 <- dat

##Experiment 2, Simon task (Data Set 6)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/StroopSimonAPP2010/allsi7.dat")
clnames <- c('sub'
             ,'blk'
             ,'blktype'
             ,'trial'
             ,'word'
             ,'location'
             ,'cond'
             ,'resp'
             ,'acc'
             ,'rt'
             ,'errorTotal')

dat <- read.table(filename)
colnames(dat) <- clnames

#clean rt data as proposed in Pratte et al., 2010
#only congruent & incongruent condition and only stroop data
dat <- dat[dat$rt > .2 & dat$rt < 2, ]
dat <- subset(dat, acc == 1 & blktype == 0) #accurate data, only simon task data
dat <- dat[!(dat$trial %in% 1:5), ]

dat.simon.p2 <- dat


