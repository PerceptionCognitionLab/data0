readData=function(fileroot,datCols,
                  colNames=c('sub','ageGroup','block','trialType','cond','stim','acc','rt'))
{
  cmd=paste("ls -1 ",fileroot," >temp",sep='')
  system(cmd)
  filenames=paste(fileroot,read.table('temp')[,1],sep='')
  I=length(filenames)
  
  dat=NULL
  for (i in 1:I){
    mysub=read.table(filenames[i])
    dat=rbind(dat,mysub)
  }
  
  dat=dat[,datCols]
  colnames(dat)=colNames
  return(dat)}

stroopCols=c(1,3,11,12,14,15,18,20)

numStroop=readData(
  "..//osfstorage-archive//data_wd//form1//numberstroop//",
  stroopCols)
write.table(numStroop,file="numStroop.dat",row.names = F,quote = F)

colStroop=readData(
  "..//osfstorage-archive//data_wd//form2//colorstroop//",
  stroopCols)
write.table(colStroop,file="colStroop.dat",row.names = F,quote = F)

letFlanker=readData(
  "..//osfstorage-archive//data_wd//form1//letterFlanker//",
  stroopCols)
write.table(letFlanker,file="letFlanker.dat",row.names = F,quote = T)

arrowFlanker=readData(
  "..//osfstorage-archive//data_wd//form2//arrowFlanker//",
  stroopCols)
write.table(arrowFlanker,file="arrowFlanker.dat",row.names = F,quote = F)

