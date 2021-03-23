
readST3=function(bounds=c(.2,5)){
  # Returns 3 versions of data set in a list
      # $raw is Raw data without cleaning
      # $clean is cleaned yet has large learning effect
      # $clean2 has first three blocks removed to control learning
  
  link="https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/rt-shiftScaleShape/st3.dat"
  dat=read.table(head=F,url(link))
  colnames(dat)=c('sub','block','trial','trialB','cond','resp','rt','acc')
  #codes
  #resp: 0=left, 1=right
  #acc: 0=error, 1=correct
  #cond: 0 easiest left, 2 is hardest left, 3 hardest right, 5 easiest right

  dat$sub=as.integer(as.factor(dat$sub))
  I=length(levels(as.factor(dat$sub))) #Number of Subs.

  wrong.button=dat$resp>1
  error=dat$acc==0
  stop=wrong.button | dat$trialB==0 |error
  after.stop=c(0,stop)[1:length(stop)]
  a=dat$rt<bounds[1]
  b=dat$rt>bounds[2]
  dont.use=stop | after.stop |a |b
  clean=dat[!dont.use,]
  clean2=clean[clean$block>2,]
  return(list(raw=dat,clean=clean,clean2=clean2))}
  
#Example, cleaned & learning artifact removed
#dat=readST3()$clean2


