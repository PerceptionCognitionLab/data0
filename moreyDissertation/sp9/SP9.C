#include "sp.h"
#include <string.h>

extern ALFONT_FONT *fontlst[];
extern long double rtSlope;
extern int errorTotal;
extern movie *std_m;
extern image *std_i;

#define backup 0
#define STATUS RUN


#define rate 120

#define FONTNUM 1

#define NTIDTRIALSPERBLOCK (NWORDS*2) 
#define NTIDBLOCKS 9
#define NPIDBLOCKS 8
#define NPIDTRIALSPERBLOCK (NWORDS*2)

#define BREAKAFTERPIDBLOCK 3 

#define NPIDDURS 6
#define NTIDDURS 3

#define NMASKCHARS 20
#define MASKLENGTH 13


extern int UseDIO24;

main()
{
  int mapWords[2*NWORDS],i=0,j=0,t=0,blk=0,cols[2*NWORDS],rows[2*NWORDS];
  int PIDdur[2*NWORDS],TIDdur[2*NWORDS],TIDcode[2*NWORDS],PIDcode[2*NWORDS];

  char maskChars[NMASKCHARS+1];
  sprintf(maskChars,"%s","BCDFGHJKLMNPQRSTVWXZ");

  int frameDurs[NFRAMES]={1,60,18,-1,2,-1,22,1};
  int tiddurs[NTIDDURS]={3,4,8};
  int piddurs[NPIDDURS]={3,4,4,8,18,18};

  logtype *log;
  FILE *outfile;
  trialParams oneTrial;
  

  FILE *inwords;
  inwords=fopen("bad.wrd","r");
  loadwordlist(0,inwords);
  fclose(inwords);

  inwords=fopen("good.wrd","r");
  loadwordlist(1,inwords);
  fclose(inwords);
 
 
  UseDIO24= 0;
  log=(logtype *)malloc(1*sizeof(logtype));
 
  initLog(STATUS,log);
  outfile=fopen(log->outfn,"w");

  setPicBG(0);
  setup(640,480,640,100,0,rate,8,"fonts.lst");
  if(alfont_set_font_size(fontlst[FONTNUM], 16)==ALFONT_ERROR)
    error("Cannot Set Font Size");
  //set up the color palette 
  makePalette(GRAYSCALE); 
  oneTrial.trial_m=newMovie(NFRAMES);
  for(i=0;i<NFRAMES;i++){
	oneTrial.stim_i[i]=newImage();
	oneTrial.stimFrameDurs[i]=frameDurs[i];
  }
  Palette[254]=colVal(128,128,255);
  Palette[253]=colVal(255,64,64);
  Palette[252]=colVal(64,255,64);
  oneTrial.xcen=329;
  oneTrial.ycen=49;
  oneTrial.fontNum=FONTNUM;
  oneTrial.minRT=.200; 
  oneTrial.fixationSize=10; 
  oneTrial.yFixAdj=8;
  oneTrial.xFixAdj=0;
  oneTrial.badFBLength=2000; //ms


  for(i=0;i<(2*NWORDS);i++){
    rows[i]=i;
    cols[i]=(i+NWORDS/2)%(2*NWORDS);
    TIDcode[i]=i;
    PIDcode[i]=i;
    mapWords[i]=i;
    TIDdur[i]=i%3;
    PIDdur[i]=i%5;
  } 
   
  oneTrial.tid=1;
  oneTrial.feedback=1;

  for(blk=0;blk<NTIDBLOCKS;blk++){ 
   
    runMovieText("NEGATIVE              Categorize FINAL word.               POSITIVE",oneTrial.xcen,oneTrial.ycen,oneTrial.fontNum,254);
    distribute(mapWords,NWORDS,&(log->seed));
    distribute(&mapWords[NWORDS],NWORDS,&(log->seed));
    distribute(TIDcode,2*NWORDS,&(log->seed));

    for(t=0;t<NTIDTRIALSPERBLOCK;t++)
      {  
	oneTrial.primeCat=mapWords[rows[TIDcode[t]]]/NWORDS;
	oneTrial.targCat=mapWords[cols[TIDcode[t]]]/NWORDS;
	oneTrial.primeNum=mapWords[rows[TIDcode[t]]]%NWORDS;
	oneTrial.targNum=mapWords[cols[TIDcode[t]]]%NWORDS;
	oneTrial.pdur=tiddurs[TIDdur[TIDcode[t]]];
	sprintf(oneTrial.targWord, "%s",words[oneTrial.targCat][oneTrial.targNum]);
	sprintf(oneTrial.primeWord,"%s",words[oneTrial.primeCat][oneTrial.primeNum]); 

	oneTrial.stimFrameDurs[PRIME]=oneTrial.pdur;
	 
	if(oneTrial.pdur>=6){
		oneTrial.stimFrameDurs[PREBLANK]=0;
	}else{
		oneTrial.stimFrameDurs[PREBLANK]=8-(oneTrial.pdur+2);
	} 


	distributeChar(maskChars,NMASKCHARS,&(log->seed));
	sprintf(oneTrial.preMask,"%s",&(maskChars[NMASKCHARS-MASKLENGTH]));
	
	
	distributeChar(maskChars,NMASKCHARS,&(log->seed));
	sprintf(oneTrial.postMask,"%s",&(maskChars[NMASKCHARS-MASKLENGTH]));
	
	doTrial(&oneTrial,&(log->seed));

	
	fprintf(outfile,"%03d %d %02d %d %d %d %02d %s %d %02d %s %d %d %s %s %f %d\n",
		log->subjnum, 
		blk,
		t,
		oneTrial.tid, 
		oneTrial.pdur,
		oneTrial.primeCat,
		oneTrial.primeNum,
		oneTrial.primeWord,
		oneTrial.targCat,
		oneTrial.targNum,
		oneTrial.targWord,
		oneTrial.response,
		oneTrial.acc,
		oneTrial.preMask,
		oneTrial.postMask,
		oneTrial.RT,
		errorTotal
		);
      fflush(outfile);
      }
  }
  runMovieTimedText("STOP. Please go to room 203.",oneTrial.xcen,oneTrial.ycen,oneTrial.fontNum,253,3600);
  
 
  //Prime Identification//

  oneTrial.tid=0;
  oneTrial.feedback=0;

  for(blk=0;blk<NPIDBLOCKS;blk++){ 
   
    runMovieText("NEGATIVE              Categorize FIRST word.               POSITIVE",oneTrial.xcen,oneTrial.ycen,oneTrial.fontNum,254);
    distribute(mapWords,NWORDS,&(log->seed));
    distribute(&mapWords[NWORDS],NWORDS,&(log->seed));
    distribute(PIDcode,2*NWORDS,&(log->seed));
    
    for(t=0;t<NPIDTRIALSPERBLOCK;t++)
      {  
	oneTrial.primeCat=mapWords[rows[PIDcode[t]]]/NWORDS;
	oneTrial.targCat=mapWords[cols[PIDcode[t]]]/NWORDS;
	oneTrial.primeNum=mapWords[rows[PIDcode[t]]]%NWORDS;
	oneTrial.targNum=mapWords[cols[PIDcode[t]]]%NWORDS;
    
	oneTrial.pdur=piddurs[PIDdur[PIDcode[t]]];
	sprintf(oneTrial.targWord, "%s",words[oneTrial.targCat][oneTrial.targNum]);
	sprintf(oneTrial.primeWord,"%s",words[oneTrial.primeCat][oneTrial.primeNum]); 


	oneTrial.stimFrameDurs[PRIME]=oneTrial.pdur;
	if(oneTrial.pdur>=6){
		oneTrial.stimFrameDurs[PREBLANK]=0;
	}else{
		oneTrial.stimFrameDurs[PREBLANK]=8-(oneTrial.pdur+2);
	} 
	
	distributeChar(maskChars,NMASKCHARS,&(log->seed));
	sprintf(oneTrial.preMask,"%s",&(maskChars[NMASKCHARS-MASKLENGTH]));
	
	distributeChar(maskChars,NMASKCHARS,&(log->seed));
	sprintf(oneTrial.postMask,"%s",&(maskChars[NMASKCHARS-MASKLENGTH]));
	
	doTrial(&oneTrial,&(log->seed));
	
	fprintf(outfile,"%03d %d %02d %d %d %d %02d %s %d %02d %s %d %d %s %s %f %d\n",
		log->subjnum, 
		blk+NTIDBLOCKS,
		t,
		oneTrial.tid,
		oneTrial.pdur,
		oneTrial.primeCat,
		oneTrial.primeNum,
		oneTrial.primeWord,
		oneTrial.targCat,
		oneTrial.targNum,
		oneTrial.targWord,
		oneTrial.response,
		oneTrial.acc,
		oneTrial.preMask,
		oneTrial.postMask,
		oneTrial.RT,
		errorTotal
		);
      fflush(outfile);
      }
    if(blk==BREAKAFTERPIDBLOCK) runMovieTimedText("STOP. Please go to room 203.",oneTrial.xcen,oneTrial.ycen,oneTrial.fontNum,253,3600);
  }

    
  runMovieTimedText("Done, thank you! Please go to room 203.",oneTrial.xcen,oneTrial.ycen,oneTrial.fontNum,252,3600);
  cleanup();
  fclose(outfile);
  
}



