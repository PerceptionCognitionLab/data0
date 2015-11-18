#include "luck.h"

extern ALFONT_FONT *fontlst[];
extern long double rtSlope;
extern int errorTotal;
extern movie *std_m;
extern image *std_i;

#define backup 0
#define STATUS RUN

#define rate 60

#define FONTNUM 2

#define NBLOCKS 9
#define TRIALSPERBLOCK 60
#define NN 3
#define NPROBS 3

extern int UseDIO24;

main()
{
  

  int i,j,k;
  float distance;
  logtype *log;
  FILE *outfile;
  luckParams luckTrial;
  float prob;
  char num[20];
  pieParams myPie;
 
  
  int trials[TRIALSPERBLOCK];

  float changeProbs[]={7,5,3,5,7,3,7,3,5};
  float denom=10;        

  int myMaskCols[]={1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6};
		
  int N[NN]={2,5,8};

  UseDIO24= 0;
  log=(logtype *)malloc(1*sizeof(logtype));
  initLog(STATUS,log);
  outfile=fopen(log->outfn,"w");

  setup(1024,768,1024,768, 0,rate,8,"fonts.lst");
  if(alfont_set_font_size(fontlst[FONTNUM], 32)==ALFONT_ERROR)
    error("Cannot Set Font Size");
  
  //set up the color palette 
  makePalette(GRAYSCALE);
  Palette[0]=colVal(127,127,127);
  Palette[1]=colVal(0,0,0);
  Palette[2]=colVal(255,255,255);
  Palette[3]=colVal(255,0,0);
  Palette[4]=colVal(0,255,0);
  Palette[5]=colVal(0,0,255);
  Palette[6]=colVal(255,0,255);
  Palette[7]=colVal(255,255,0);
  Palette[8]=colVal(0,255,255);
  Palette[9]=colVal(204,102,0);
  Palette[10]=colVal(0,102,102);
  
 

  //initialize everything that will remain constant
  //throughout the experiment
  //reserve image and movie memory
  luckTrial.trial_m=newMovie(6);
  luckTrial.feedback_m=newMovie(1);
  luckTrial.fixate_i=newImage();
  luckTrial.blank_i=newImage();
  luckTrial.probe_i=newImage();
  luckTrial.first_i=newImage();
  luckTrial.feedback_i=newImage();
  luckTrial.mask_i=newImage();
  //image durations
  luckTrial.predur=.2*rate;
  luckTrial.firstdur=.5*rate;
  luckTrial.interdur=.5*rate;
  luckTrial.probedur=.4*rate;
  luckTrial.fixatedur=1*rate;
  luckTrial.maskdur=.5*rate;

  //other stuff
  luckTrial.xcen=1024/2;
  luckTrial.ycen=768/2;
  luckTrial.ySize=201;
  luckTrial.xSize=271;
  luckTrial.rPanelxcen=1024*.75;
  luckTrial.rPanelycen=luckTrial.ycen;
  luckTrial.lPanelxcen=1024*.25;
  luckTrial.lPanelycen=luckTrial.ycen=768/2;
  luckTrial.squareSize=20;
  luckTrial.probeColor=1;
  luckTrial.probeScale=2;
  luckTrial.minDist=58;
  luckTrial.fontNum=FONTNUM;
  luckTrial.maxColor=10;
  luckTrial.feedback=1;//auditory feedback
  luckTrial.doProbe=0;
  luckTrial.minRT=.200;
  luckTrial.fixateScale=2;
  luckTrial.fixateColor=1;
  luckTrial.feedbackLineColor=0;
  luckTrial.doFeedbackLine=1;
  luckTrial.drawNonCued=0;
  luckTrial.maskDivNum=5;
  luckTrial.maskCols=myMaskCols;
  luckTrial.trialType=1;
  luckTrial.twoPanelFeedback=0; //0=never,1=only on incorrects,2=always  
  //luckTrial.twoPanelBox=1; //0 for no box, 1 for box 
  //luckTrial.boxScale=1.1;  
  luckTrial.boxScale=1.1;  
  

  luckTrial.twoPaneldur=2*rate;
  luckTrial.twoPanelBoxColor=1;

  myPie.pie_m=newMovie(1);
  myPie.pie_i=newImage();
  myPie.x=luckTrial.xcen;
  myPie.y=luckTrial.ycen;
  myPie.scale=1;
  myPie.lineColor=1;
  myPie.color1=2;
  myPie.color2=5;
  myPie.piedur=2*rate;
  myPie.textColor=1;
  myPie.fontNum=FONTNUM;

  //log->seed=-1507;


  for(j=0;j<TRIALSPERBLOCK;j++){
    trials[j]=j;
  }

  int practChange[16]={0,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0};
  int practN[16]={0,0,0,0,1,0,2,0,2,1,1,0,2,0,1,2}; 
  
  
  runMovieText("Press any key to begin practice.",luckTrial.xcen, luckTrial.ycen, luckTrial.fontNum, 1);
  

  //Practice
  for(i=0;i<16;i++){
    runMovieTimedText("Practice", luckTrial.xcen,luckTrial.ycen,FONTNUM,1,1*rate);
    luckTrial.N=N[practN[i]];
    luckTrial.isChange=practChange[i];
    doLuckTrial(&luckTrial,&(log->seed));
  }



  for(i=0;i<NBLOCKS;i++){
    prob=changeProbs[(log->subjnum%9+i)%9];
    sprintf(num,"%2.0f%% change",100*(denom-prob)/denom);      
    myPie.proportion=(denom-prob)/denom;
    drawPie(&myPie);
    distribute(trials,TRIALSPERBLOCK,&(log->seed));
    for(j=0;j<TRIALSPERBLOCK;j++){
      runMovieTimedText(num, luckTrial.xcen,luckTrial.ycen,FONTNUM,1,1*rate);
      luckTrial.N=N[trials[j]%NN];
      luckTrial.trialType=1;//(trials[j]/NN)%2;
      luckTrial.isChange=1-(trials[j]<((TRIALSPERBLOCK*prob)/denom));
      doLuckTrial(&luckTrial,&(log->seed));
      fprintf(outfile,"%d %d %d %f %d %d %d %d %f %d %d ",
	      log->subjnum,
	      i,
	      j,
	      (denom-prob)/denom,
	      luckTrial.drawNonCued,
	      luckTrial.N,
	      luckTrial.isChange,
	      luckTrial.response,
	      luckTrial.RT,
	      luckTrial.probeSquare,
	      luckTrial.newColor
	      );
      for(k=0;k<N[NN-1];k++){
	if(k<luckTrial.N){
	  fprintf(outfile,"%d %d %d ",luckTrial.colors[k],luckTrial.xlocations[k],luckTrial.ylocations[k]);
	}else{
	  fprintf(outfile,"NA NA NA ");
	}
      }
      fprintf(outfile,"\n");
    }
  }



  runMovieText("End. Please go to room 203.", luckTrial.xcen,luckTrial.ycen,FONTNUM,1);
  cleanup();

  fclose(outfile);
 
}

