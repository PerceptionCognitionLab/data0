
loadData=function(){
  # Loads and Cleans Rouder et al., 2004, PBR, Lexical Distance-From-Five data
  link<-"https://raw.githubusercontent.com/PerceptionCognitionLab/data0/master/lexDec-dist5/ld5.all"
  dat<-read.table(url(link),head=FALSE)
  colnames(dat) <- c("sub","block","trial","stim","resp","rt","err")
  dig<-c(2,3,4,6,7,8)
  dat$digit<-dig[dat$stim+1]
  dat$totTrial <- dat$block*60+dat$trial+1
  badSub <- dat$sub %in% c(34,43)
  errs <- dat$err==1
  tooFast <- dat$rt<200
  tooSlow <- dat$rt>2000
  warm <- dat$totTrial<20
  first <- dat$trial==0
  badRows <- badSub | errs | tooFast | tooSlow | warm | first
  clean=dat[!badRows,]
  clean$sub <- as.integer(as.factor(clean$sub))
  return(clean)
  }
