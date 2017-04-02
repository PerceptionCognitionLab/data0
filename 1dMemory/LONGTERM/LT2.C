#include <stdio.h>       
#include <stdlib.h>
#include <string.h> /*for memcpy command*/
#include "movieobj.h"
#include "video.h"

#include <mem.h>
#include "exptlib.h"


#define status STAND_ALONE
#define numtrials 650
#define trialsperblock 65
#define numblocks ((numtrials)/(trialsperblock))
#define backup 0
#define fore_time 30
#define delta 11

void hline(int len, unsigned char far *b,long *seed);
void vhline(int len, int y, unsigned char far *b,long *seed);



/*int length[12]={23,39,57,77,99,123,148,174,201,230,260,291}; old*/
int length[13]={15,23,32,43,57,73,93,116,143,174,210,251,298};

main()
{
  int *stim;
  int b,s,t;

  logtype *log;
  FILE *outfile,*backfile;
  char *fontlist,junk[3];
  char backstr[40],outline[80];
  long seed;

  image *wrong_i,*blank, *targ_i;
  movie *wrong_m,*ready_m,*stim_m,*feed_m;
  image *ready13_i;
  response *data,*data2; int resp,second; long RT;

  fontlist=getenv("FONTLIST");
  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  SetupMoviePackage(fontlist);
  seed=log->seed;
  outfile=fopen(log->outfn,"wt");
  if (backup)
    {
     sprintf(backstr,"c:\\expbak\\%s",log->outfn);
     backfile=fopen(backstr,"wt");
    }

  stim= (int *)malloc(sizeof(int)*numtrials);
  for (t=0;t<numtrials;t++) 
    stim[t]=t%13;
  distribute(stim,numtrials,&seed);

 /* set up pallete here*/
  makePalette(GRAYSCALE);
  /*READY movie*/
  /*wrong movie*/
  wrong_i=newImage();
  downloadImage(wrong_i);
  drawText("invalid key",0,0,0,255);
  uploadImage(wrong_i);
  wrong_m=initMovie(1);
  setMovie(wrong_m,0,wrong_i,24);

  ready13_i=newImage();
  downloadImage(ready13_i);
  for (s=0;s<13;s++)
  {
    sprintf(junk,"%d",s);
    vhline(length[s],(s*16),picBuf,&seed);
    drawText(junk,xcenter,(s*16)+2,0,255);
  }
  uploadImage(ready13_i);

  ready_m=initMovie(1);



  stim_m=initMovie(2);
  feed_m=initMovie(1);
  blank=newImage();
  targ_i=newImage();
  setMovie(stim_m,0,blank,fore_time);



  for (b=0;b<numblocks;b++)
    {
      setMovie(ready_m,0,ready13_i,1);
      runMovie(ready_m,UNTIL_RESPONSE,1);
      for (s=0;s<trialsperblock;s++)
	{
	  t=s+(b*(trialsperblock));
	  downloadImage(targ_i);
	  clearPicBuf();
	  hline(length[stim[t]],picBuf,&seed);
	  uploadImage(targ_i);
	  setMovie(stim_m,1,targ_i,1);
	  data=runMovie(stim_m,UNTIL_RESPONSE,1);
	  RT=data->x[0].rt-(msperframe*fore_time);
	  switch (data->x[0].resp){
	  case '1': resp=1;break;
	  case '2': resp=2;break;
	  case '3': resp=3;break;
	  case '4': resp=4;break;
	  case '5': resp=5;break;
	  case '6': resp=6;break;
	  case '7': resp=7;break;
	  case '8': resp=8;break;
	  case '9': resp=9;break;
	  case '0': resp=10;break;
	  case '-': resp=11;break;
	  case '=': resp=12;break;
	  case '`': resp=0;break;
	  case '@': resp=13;break;
	  default : resp=14;audio(INVALID,wrong_m);break;}
	  if (resp==stim[t]) 
	    { audio(CORRECT);
	      second = -1;
	    }
	  else
	    {
	      downloadImage(targ_i);
	      drawText("Try Again",xcenter-10,ycenter+40,0,255);
	      uploadImage(targ_i);
	      setMovie(feed_m,0,targ_i,1);
	      data2=runMovie(feed_m,UNTIL_RESPONSE,1);
	      switch (data2->x[0].resp){
	      case '1': second=1;break;
	      case '2': second=2;break;
	      case '3': second=3;break;
	      case '4': second=4;break;
	      case '5': second=5;break;
	      case '6': second=6;break;
	      case '7': second=7;break;
	      case '8': second=8;break;
	      case '9': second=9;break;
	      case '0': second=10;break;
	      case '-': second=11;break;
	      case '=': second=12;break;
	      case '`': second=0;break;
	      case '@': second=13;break;
	      default : second=14;audio(INVALID,wrong_m);break;}
	      if (second==stim[t]) 
		{ audio(CORRECT);}
	     else
	       {
		sprintf(junk,"%d",(stim[t]));
		downloadImage(targ_i);
		drawText(junk,xcenter-10,ycenter+60,0,255);
		uploadImage(targ_i);
		setMovie(feed_m,0,targ_i,30);
		runMovie(feed_m,FULL_SHOW,0);
		}
	    }
	  sprintf(outline,"sub%03d  blk%02d  trl%03d  %02d %02d %02d %05d\n",
		  log->subjnum,b,s,stim[t],resp,second,RT);
	  fprintf(outfile, "%s",outline);
	  if (backup) fprintf(backfile,"%s",outline);
	  if (resp==13) {fclose(outfile);CleanupMoviePackage();
	  printf ("stopped while running by participant\n");exit(1);}
	}
    }
  fclose(outfile);
  if (backup) fclose(backfile);
  thankyou();
  CleanupMoviePackage();
}


	  





  /*-----------------------------------------*/










void vhline(int len, int y, unsigned char far *b,long *seed)
{
  int q, yrnd,xstart;
  char junk[3];
  
  yrnd=y;
  xstart=xcenter-(len/2);
  for (q=xstart;q<xstart+len;q++) b[o(q,yrnd)]=255;
}



void hline(int len, unsigned char far *b,long *seed)
{
  int q, yrnd,xstart;
  char junk[3];
  
  yrnd=ycenter+randint(-delta,delta,seed);
  xstart=xcenter-(len/2)+randint(-delta,delta,seed);
  for (q=xstart;q<xstart+len;q++) b[o(q,yrnd)]=255;
}


