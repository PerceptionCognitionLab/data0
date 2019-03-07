#include "np.h"
#include <string.h>

extern ALFONT_FONT *fontlst[];
extern long double rtSlope;
extern int errorTotal;
extern movie *std_m;
extern image *std_i;

#define backup 0
#define STATUS RUN

#define NSTIMS 6

#define rate 120

#define FONTNUM 1

#define TOTALSOAFRAMES 19

#define NTRIALSPERBLOCK 60
#define NDURS 3
#define NBLOCKS 9 
#define NTIDTRIALS NTRIALSPERBLOCK*NBLOCKS

#define NPIDBLOCKS 4
#define NPIDTRIALSPERBLOCK 100
#define NPIDTRIALS NPIDBLOCKS*NPIDTRIALSPERBLOCK
#define NPIDDURS 5

extern int UseDIO24;

main()
{

  int tCounter=0,im=0,i=0,j=0,t=0,blk=0,code[NTIDTRIALS],pidcode[NPIDTRIALS],myPrimedurs[NDURS],mypiddurs[NPIDDURS];
  logtype *log;
  FILE *outfile;
  trialParams oneTrial;
  int stims[NSTIMS]={2,3,4,6,7,8};
  
  UseDIO24= 0;
  log=(logtype *)malloc(1*sizeof(logtype));
 
  initLog(STATUS,log);
  outfile=fopen(log->outfn,"w");
 
  int cbal=log->subjnum%2;
  int group=log->subjnum/2;
  
  srand(group);
  
  int primedurs[2][NDURS+1]={ 
			   {2,4,6,12}, 
			   {1,3,5,12} 
			  }; 

  for(i=0;i<NDURS;i++){
      myPrimedurs[i]=primedurs[((rand()%2)+cbal)%2][i];
  }

  for(i=0;i<NPIDDURS;i++){
    if(i<NDURS){
      mypiddurs[i]=myPrimedurs[i];
      }else{
      mypiddurs[i]=primedurs[1][NDURS];
      }
  }


  setPicBG(0);

  setup(640,480,640,480,255,rate,8,"fonts.lst");
  if(alfont_set_font_size(fontlst[FONTNUM], 16)==ALFONT_ERROR)
    error("Cannot Set Font Size");
  
  
  //set up the color palette 
  makePalette(GRAYSCALE); 
  //initialize everything that will remain constant
  //throughout the experiment
  // reserve image and movie memory
  int frameDurs[NFRAMES]={5,60,7,-1,7,480,1};

  oneTrial.trial_m=newMovie(NFRAMES);
 
  oneTrial.xcen=319;
  oneTrial.ycen=239;
  oneTrial.fontNum=FONTNUM;
  oneTrial.minRT=.200; 
  oneTrial.fixationSize=7; 
  oneTrial.fixYOffset=7;
  oneTrial.feedback=1;
  oneTrial.wrongSoundLength=2000;

  for(i=0;i<NFRAMES;i++){
	oneTrial.stim_i[i]=newImage();
	oneTrial.stimFrameDurs[i]=frameDurs[i];
  }
  
  for(i=0;i<NTIDTRIALS;i++){
    code[i]=i%(NDURS*NSTIMS*(NSTIMS-1));
  }
  for(i=0;i<NPIDTRIALS;i++){  
    pidcode[i]=i%(NPIDDURS*NSTIMS*(NSTIMS-1));
  }
  

  tCounter=0;    
  distribute(code,NTIDTRIALS,&(log->seed));
  oneTrial.targetID=1;
  
  
  for(blk=0;blk<NBLOCKS;blk++){

  //oneTrial.stimFrameDurs[PREBLANK]=TOTALSOAFRAMES-oneTrial.stimFrameDurs[PRIME]-oneTrial.stimFrameDurs[POSTMASK];

	runMovieText("Is final number less than or greater than 5?",oneTrial.xcen,oneTrial.ycen,1,255);
	for(t=0;t<NTRIALSPERBLOCK;t++)
	{  
	  oneTrial.stimFrameDurs[PRIME]=myPrimedurs[code[tCounter]/(NSTIMS*(NSTIMS-1))];
	  oneTrial.primeNum=code[tCounter]%NSTIMS;
	  oneTrial.targetNum=(code[tCounter]%(NSTIMS*(NSTIMS-1)))/NSTIMS;
	  if(oneTrial.targetNum>=oneTrial.primeNum) oneTrial.targetNum+=1;
	  oneTrial.primeNum=stims[oneTrial.primeNum];
	  oneTrial.targetNum=stims[oneTrial.targetNum];

	  doTrial(&oneTrial,&(log->seed));
	  
	  fprintf(outfile,"%03d %d %02d %d %d %d %d %d %d %f %d\n",
		  log->subjnum, 
		  blk,
		  t,
		  oneTrial.targetID,
		  oneTrial.stimFrameDurs[PRIME],
		  oneTrial.primeNum,
		  oneTrial.targetNum,
		  oneTrial.response,
		  oneTrial.cor,
		  oneTrial.RT,
		  errorTotal  
		  );
	  tCounter++;
	}
  }
  

  runMovieTimedText("Please STOP and go to room 203 now.",oneTrial.xcen,oneTrial.ycen,1,255,30*rate);
  

  //Prime ID blocks
  oneTrial.targetID=0;
  oneTrial.wrongSoundLength=300;
  oneTrial.feedback=0;

  
  distribute(pidcode,NPIDTRIALS,&(log->seed));
  
  tCounter=0;

  for(blk=NBLOCKS;blk<NBLOCKS+NPIDBLOCKS;blk++){
  

	runMovieText("Is FIRST number less than or greater than 5?",oneTrial.xcen,oneTrial.ycen,1,255);
	for(t=0;t<NPIDTRIALSPERBLOCK;t++)
	{  
	  oneTrial.stimFrameDurs[PRIME]=mypiddurs[pidcode[tCounter]/(NSTIMS*(NSTIMS-1))];
	  //oneTrial.stimFrameDurs[PREBLANK]=TOTALSOAFRAMES-oneTrial.stimFrameDurs[PRIME]-oneTrial.stimFrameDurs[POSTMASK];
	  oneTrial.primeNum=pidcode[tCounter]%NSTIMS;
	  oneTrial.targetNum=(pidcode[tCounter]%(NSTIMS*(NSTIMS-1)))/NSTIMS;
	  if(oneTrial.targetNum>=oneTrial.primeNum) oneTrial.targetNum+=1;
	  oneTrial.primeNum=stims[oneTrial.primeNum];
	  oneTrial.targetNum=stims[oneTrial.targetNum];

	  doTrial(&oneTrial,&(log->seed));
	  
	  fprintf(outfile,"%03d %d %02d %d %d %d %d %d %d %f %d\n",
		  log->subjnum, 
		  blk,
		  t,
		  oneTrial.targetID,
		  oneTrial.stimFrameDurs[PRIME],
		  oneTrial.primeNum,
		  oneTrial.targetNum,
		  oneTrial.response,
		  oneTrial.cor,
		  oneTrial.RT,
		  errorTotal
		  );
	  fflush(outfile);
	  tCounter++;
	}
  }
     
 

  
  runMovieText("Done. Thank you!",oneTrial.xcen,oneTrial.ycen,1,255);
  
  cleanup();
  
  fclose(outfile);
  
}

