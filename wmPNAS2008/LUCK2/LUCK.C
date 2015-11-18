#include "luck.h"
extern long double rtSlope;


int colVal(int red,int green,int blue){
	return((red*256*256)+(256*green)+(blue));
}


int doLuckTrial(luckParams *luckTrial, int *seed){
	  
  int N=luckTrial->N;
  int i,j,k,xx,yy;
  int badLocation=0;
  float distance,RT;
  //char num[2];
  int respChange,tooFast=0,repeat=0;
  int myColors[luckTrial->maxColor];
  for(i=0;i<luckTrial->maxColor;i++){
    myColors[i]=i+1;
  }
  

  //clear the images
  clearImage(luckTrial->blank_i);
  clearImage(luckTrial->fixate_i);
  clearImage(luckTrial->first_i);
  clearImage(luckTrial->probe_i);
  clearImage(luckTrial->feedback_i);
  clearImage(luckTrial->mask_i);

  //fixation
  line(*(luckTrial->fixate_i),luckTrial->xcen,luckTrial->ycen+5*luckTrial->fixateScale,luckTrial->xcen,luckTrial->ycen-5*luckTrial->fixateScale,luckTrial->fixateColor);
  line(*(luckTrial->fixate_i),luckTrial->xcen+5*luckTrial->fixateScale,luckTrial->ycen,luckTrial->xcen-5*luckTrial->fixateScale,luckTrial->ycen,luckTrial->fixateColor);

  //do the stuff needed for a change trial (choose which will change, new color, etc)
  if(luckTrial->isChange){
    luckTrial->newColor=randint(1,luckTrial->maxColor-1,seed);
    luckTrial->changedSquare=randint(0,luckTrial->N-1,seed);
    luckTrial->probeSquare=luckTrial->changedSquare;
  }else{
    //if nothing will change, choose a random square to probe
    luckTrial->probeSquare=randint(0,luckTrial->N-1,seed);
  }
  
  //Color code
  if(luckTrial->trialType){
     distribute(myColors,luckTrial->maxColor,seed);
    for(i=0;i<N;i++){
      luckTrial->colors[i]=myColors[i]; 
    }
    luckTrial->newColor=myColors[luckTrial->maxColor-1];
  }else{
    for(i=0;i<N;i++){
      luckTrial->colors[i]=randint(1,luckTrial->maxColor,seed);
      if(i && !repeat) for(j=0;j<i;j++){
	if(luckTrial->colors[i]==luckTrial->colors[j]) repeat=1;
      }
  
    }
    if(!repeat) luckTrial->colors[0]=luckTrial->colors[luckTrial->N-1];
  }

  if(!luckTrial->isChange) luckTrial->newColor=luckTrial->colors[luckTrial->probeSquare];

  for(i=0;i<N;i++){
    //choose a location and check it. If it is no good, choose another location.
    do{
      badLocation=0;
      //choose location
      luckTrial->xlocations[i]=randint(luckTrial->xcen-luckTrial->xSize/2+luckTrial->squareSize/2,luckTrial->xcen+luckTrial->xSize/2-luckTrial->squareSize/2,seed);
      luckTrial->ylocations[i]=randint(luckTrial->ycen-luckTrial->ySize/2+luckTrial->squareSize/2,luckTrial->ycen+luckTrial->ySize/2-luckTrial->squareSize/2,seed);       
      //check distance from center
      distance=sqrt(pow(luckTrial->xlocations[i]-luckTrial->xcen,2)+pow(luckTrial->ylocations[i]-luckTrial->ycen,2));
      if(distance<luckTrial->minDist){ 
	badLocation=1;
      }else if(i>0){
	for(j=0;j<i;j++){
	  //check distance from all other squares
	  distance=sqrt(pow(luckTrial->xlocations[i]-luckTrial->xlocations[j],2)+pow(luckTrial->ylocations[i]-luckTrial->ylocations[j],2));
	  if(distance<luckTrial->minDist){
	    badLocation=1;
	    break;
	  }
	}
      }
    }while(badLocation==1);
    
    //draw the squares
    rectfill(*(luckTrial->first_i),luckTrial->xlocations[i]-luckTrial->squareSize/2,luckTrial->ylocations[i]-luckTrial->squareSize/2,luckTrial->xlocations[i]+luckTrial->squareSize/2,luckTrial->ylocations[i]+luckTrial->squareSize/2,luckTrial->colors[i]);
    
    for(j=0;j<pow(luckTrial->squareSize,2);j++){          
      xx=(j%luckTrial->squareSize);
      yy=j/luckTrial->squareSize;
      //k=1;
      k=luckTrial->maskCols[
			    luckTrial->squareSize/luckTrial->maskDivNum*(yy/luckTrial->maskDivNum)+
			    (xx/luckTrial->maskDivNum)
      ];
      putpixel(*(luckTrial->mask_i),
	       luckTrial->xlocations[i]-luckTrial->squareSize/2+xx,
	       luckTrial->ylocations[i]-luckTrial->squareSize/2+yy,
	       k);   
    }
    
    if(luckTrial->drawNonCued||(!(luckTrial->drawNonCued)&&luckTrial->probeSquare==i)){
	rectfill(*(luckTrial->probe_i),luckTrial->xlocations[i]-luckTrial->squareSize/2,luckTrial->ylocations[i]-luckTrial->squareSize/2,luckTrial->xlocations[i]+luckTrial->squareSize/2,luckTrial->ylocations[i]+luckTrial->squareSize/2,luckTrial->colors[i]);
    } 
    rectfill(*(luckTrial->feedback_i),luckTrial->xlocations[i]-luckTrial->squareSize/2,luckTrial->ylocations[i]-luckTrial->squareSize/2,luckTrial->xlocations[i]+luckTrial->squareSize/2,luckTrial->ylocations[i]+luckTrial->squareSize/2,luckTrial->colors[i]);
    
    //draw the changed square
    if(luckTrial->isChange && luckTrial->changedSquare==i){
      if(luckTrial->colors[i]==luckTrial->newColor) luckTrial->newColor=luckTrial->maxColor;
      rectfill(*(luckTrial->probe_i),luckTrial->xlocations[i]-luckTrial->squareSize/2,luckTrial->ylocations[i]-luckTrial->squareSize/2,luckTrial->xlocations[i]+luckTrial->squareSize/2,luckTrial->ylocations[i]+luckTrial->squareSize/2,luckTrial->newColor);
     
      }
    

    //sprintf(num,"%i",i);
    //drawText(luckTrial->probe_i,num,luckTrial->xlocations[i],luckTrial->ylocations[i]-5,luckTrial->fontNum,0);
    //drawText(luckTrial->first_i,num,luckTrial->xlocations[i],luckTrial->ylocations[i]-5,luckTrial->fontNum,0);    
    
    //draw the probe circle, if necessary
    if(luckTrial->doProbe&&luckTrial->probeSquare==i){
      circle(*(luckTrial->probe_i),luckTrial->xlocations[luckTrial->probeSquare],luckTrial->ylocations[luckTrial->probeSquare],(luckTrial->probeScale)*(luckTrial->squareSize/2),luckTrial->probeColor);
      circle(*(luckTrial->feedback_i),luckTrial->xlocations[luckTrial->probeSquare],luckTrial->ylocations[luckTrial->probeSquare],(luckTrial->probeScale)*(luckTrial->squareSize/2),luckTrial->probeColor);

    }
  }
  

  rectfill(*(luckTrial->feedback_i),
	   luckTrial->xlocations[luckTrial->probeSquare],
	   luckTrial->ylocations[luckTrial->probeSquare]-luckTrial->squareSize/2,
	   luckTrial->xlocations[luckTrial->probeSquare]+luckTrial->squareSize/2,
	   luckTrial->ylocations[luckTrial->probeSquare]+luckTrial->squareSize/2,
	   luckTrial->newColor);
  if(luckTrial->doFeedbackLine){
  line(*(luckTrial->feedback_i),luckTrial->xlocations[luckTrial->probeSquare],luckTrial->ylocations[luckTrial->probeSquare]+luckTrial->squareSize/2, luckTrial->xlocations[luckTrial->probeSquare],luckTrial->ylocations[luckTrial->probeSquare]-luckTrial->squareSize/2,luckTrial->feedbackLineColor);
  }


  //run the movie
  setMovie(luckTrial->trial_m,0,luckTrial->blank_i,luckTrial->predur);
  setMovie(luckTrial->trial_m,1,luckTrial->fixate_i,luckTrial->fixatedur);
  setMovie(luckTrial->trial_m,2,luckTrial->first_i,luckTrial->firstdur);
  setMovie(luckTrial->trial_m,3,luckTrial->blank_i,luckTrial->interdur);
  setMovie(luckTrial->trial_m,4,luckTrial->mask_i,luckTrial->maskdur);
  setMovie(luckTrial->trial_m,5,luckTrial->probe_i,luckTrial->probedur);
  //setMovie(luckTrial->trial_m,4,luckTrial->blank_i,luckTrial->postdur);
  luckTrial->data=runMovie(luckTrial->trial_m,UNTIL_RESPONSE,1);
  //luckTrial->data=runMovie(luckTrial->trial_m,FULL_SHOW,0);
  
  
  if ((luckTrial->trial_m->x[5].ts)==0) RT=-1;
  else RT=(luckTrial->data->x[0].rt-luckTrial->trial_m->x[5].ts)*rtSlope;
  if (RT<luckTrial->minRT)
    {
      sound(75);
      delay(4000);
      nosound();
      runMovieText("Too fast. Press any key to continue.",luckTrial->xcen, luckTrial->ycen, luckTrial->fontNum, 1);
      tooFast=1;
    }


  //process the response
  switch(luckTrial->data->x[0].resp & 0x00ff){
  case 'z': respChange=0; break;
  case '/': respChange=1; break;
  case 'Z': respChange=0; break;
    case '@': 
    cleanup();
    printf ("stopped while running by participant\n");
    exit(1);
    break;
  default: 
    respChange=-1;
    sound(75);
    delay(1000);
    nosound();
    runMovieText("Invalid response. Press any key to continue.",luckTrial->xcen, luckTrial->ycen, luckTrial->fontNum, 1);
    break;
  }  
  
  //give feedback if we want it
  if(luckTrial->feedback && respChange!=-1 && !tooFast){
    if(respChange==luckTrial->isChange){
      audio(CORRECT);
    }else{
      audio(ERROR);
    }
  }
  

  
  luckTrial->response=respChange;
  luckTrial->RT=RT;

  //luckTrial->response=1;
  //luckTrial->RT=.01;


  if(luckTrial->twoPanelFeedback>0){
    if(!((luckTrial->twoPanelFeedback==1)&&(respChange==luckTrial->isChange))){  
      setMovie(luckTrial->feedback_m,0,luckTrial->feedback_i,luckTrial->twoPaneldur);
      runMovie(luckTrial->feedback_m,UNTIL_RESPONSE,1);
    }
  }

 return(0);
}

int drawPie(pieParams *myPie){
  int x,y;
  float r,theta;
  char prob1[20],prob2[20],type1[20],type2[20];
  sprintf(prob1,"%2.0f%%",100*myPie->proportion);      
  sprintf(prob2,"%2.0f%%",100*(1-myPie->proportion));      
  sprintf(type1,"change");      
  sprintf(type2,"same");      


  float textPlaceScale1=.66;
  float textPlaceScale2=1.3;
  float textPlaceScale3=1.5;

  
  r=100*myPie->scale;
  theta=myPie->proportion*2*PI;
  clearImage(myPie->pie_i);
  circle(*(myPie->pie_i),myPie->x,myPie->y,r,myPie->lineColor);
  line(*(myPie->pie_i),myPie->x,myPie->y,myPie->x,myPie->y-r,myPie->lineColor);
  line(*(myPie->pie_i),myPie->x,myPie->y,myPie->x+r*sin(theta),myPie->y-r*cos(theta),myPie->lineColor);
  floodfill(*(myPie->pie_i),myPie->x+1,myPie->y-r/2,myPie->color1);
  floodfill(*(myPie->pie_i),myPie->x-1,myPie->y-r/2,myPie->color2);
  drawText(myPie->pie_i,prob1,myPie->x+(r*textPlaceScale1)*sin(theta/2),myPie->y-(r*textPlaceScale1)*cos(theta/2),myPie->fontNum,myPie->textColor);
  drawText(myPie->pie_i,prob2,myPie->x+(r*textPlaceScale1)*sin(PI+theta/2),myPie->y-(r*textPlaceScale1)*cos(PI+theta/2),myPie->fontNum,myPie->textColor);
  drawText(myPie->pie_i,type1,myPie->x+(r*textPlaceScale2)*sin(theta/2),myPie->y-(r*textPlaceScale2)*cos(theta/2),myPie->fontNum,myPie->textColor);
  drawText(myPie->pie_i,type2,myPie->x+(r*textPlaceScale2)*sin(PI+theta/2),myPie->y-(r*textPlaceScale2)*cos(PI+theta/2),myPie->fontNum,myPie->textColor);

  drawText(myPie->pie_i,"Press any key to begin the new block.",myPie->x,(myPie->y+r*textPlaceScale3),myPie->fontNum,myPie->textColor);

  setMovie(myPie->pie_m,0,myPie->pie_i,myPie->piedur);
  runMovie(myPie->pie_m,UNTIL_RESPONSE,1);  
  //runMovie(myPie->pie_m,FULL_SHOW,0);  

}
