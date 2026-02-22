
processIleriData=function(dat){
  cond=as.integer(factor(dat$trialtype)) # 1 is congruent, 2 is incongruent
  rt=dat$latency
  sub=as.integer(factor(dat$subject))
  subOriginal=dat$subject
  age=dat$age
  temp=as.integer(factor(dat$blockcode))
  # 1: baselineColor
  # 2: baselineWord
  # 3: proactiveColor
  #4 : proactiveWord
  #5: reactiveColor
  #6: reactiveWord
  
  isColor=(temp)%%2
  taskType=(temp-1)%/%2 
  # 0:  baseline, 1: proactive, 2: reactive
  itemType= as.integer(factor(dat$itemtype))-1 # 0: diagnostic, 1: inducer
  
  select=data.frame(sub,subOriginal,age,taskType,itemType,cond,rt,isColor)
  colnames(select)=c("sub","subOriginal","age","taskType","itemType","cond","rt","isColor")
  select$task=(select$taskType*2+select$itemType+1)+6*(1-isColor)
  #1, baseline diagnostic
  #2, baseline inducer
  #3, proactive diagnostic
  #4, proactive inducer
  #5, reactive diagnostic
  #6 reactive inducer
  #7-12  same for word identification
  select$y=select$rt
  ageCode=as.integer(as.factor(select$age))
  remap <- c(2,3,1)
  select$ageCode=remap[ageCode] #1 is young, 2, is mid, 3, is old
  return(select)
}

loadIleriData=function(){
  link="https://raw.githubusercontent.com/PerceptionCognitionLab/data0/refs/heads/master/Ileri-Tayar-JEPG-2025/alldata_excluded_combined.csv"
  processIleriData(read.csv(url(link)))
}

