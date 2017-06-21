######Pratte&Rouder, 2009, experiment 2
library(curl) ##You will need to load the R package "curl" to use this cleaning code

##Experiment 1, Stroop task (Data Set 2)
filename <- curl("https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/contexteffects/numberSubPriming/allnp11")

dat <- read.table(filename)

colnames(dat) <- c('sub','blk','blktyp'
                   ,'TIcond','primecond','trl'
                   ,'prime','primedur','targ'
                   ,'resp','cond','rt','acc','error')

dat$TIcond <- as.factor(dat$TIcond)
dat$primecond <- as.factor(dat$primecond)
dat$primedur <- as.factor(dat$primedur)

levels(dat$TIcond) <- c("Pure","Mixed")
levels(dat$primecond) <- c("16.7 ms(1)","16.7 ms(2)","18 ms","20 ms")
levels(dat$primedur) <- c("subliminal","superliminal")

dat$rep <- ifelse(dat$prime == dat$targ, 1, 0) ###dat$rep=1 if prime==targ
dat <- subset(dat, resp != 3 & blktyp==0 & TIcond == "Pure" & rep == 0)

######################
####GO CLEANING#######
######################

#Too fast
sort(table(dat$sub[dat$rt < .2]))
#Participants 36, 28, 20 are too hast more than 10 times. Remove?

#Too slow
sort(table(dat$sub[dat$rt > 2]))
#Participants 2, 58 are too slow more than 10 times. Remove?

#Inaccurate
sort(tapply(dat$acc, dat$sub, mean))
#Participant 2 is accurate in less than 80% of the cases. Remove?

#Learning
rtpBlock <- tapply(dat$rt, dat$blk, mean)
# plot(rtpBlock)
#First block is slower

block1 <- subset(dat, blk == 0)
rtpTrial1 <- tapply(block1$rt, block1$trl, mean)
# plot(rtpTrial1, pch = 19, col = "darkgray")
#First 20 trials from block 1 should be excluded

blockRest <- subset(dat, blk != 0)
rtpTrial <- tapply(blockRest$rt, blockRest$trl, mean)
# lines(rtpTrial, col = "darkred")
#Other blocks seem to be fine

#exclude too fast, too slow, inaccurate, first 20 trials
datclean <- subset(dat, rt > .2 
                   & rt < 2 
                   & acc == 1 
                   # & !(blk == 0 & trl < 19)
                   )

removed <- (nrow(dat) - nrow(datclean)) / nrow(dat)

###For investigating Individual differences, we decided to exclude the prime presentation condition with a duration of 20 ms
datclean <- subset(datclean, primecond != "20 ms")
