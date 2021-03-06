#include <stdio.h>       
#include <stdlib.h>
#include <string.h> /*for memcpy command*/
#include "movieobj.h"
#include "video.h"

#include <mem.h>
#include "exptlib.h"


#define status STAND_ALONE
#define numtrials 756
#define numblocks 12
#define backup 0
#define fore_time 30
#define delta 11

void setstim(int *stim, long *seed);
void hline(int len, unsigned char far *b,long *seed);
void vhline(int len, int y, unsigned char far *b,long *seed);


/*int length[12]={23,39,57,77,99,123,148,174,201,230,260,291}; old*/
int length[11]={23,39,57,77,99,123,148,174,201,230,260};

int map[7]={0,1,4,5,6,9,10};
int trialsperblock[numblocks]={63,63,63,63,63,63,63,63,63,63,63,63};
int blockorder[numblocks]={3,3,3,1,1,1,2,2,2,4,4,4};
int freq[7]={100,200,0,0,0,1600,3200};
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
  movie *wrong_m,*ready_m,*stim_m,*feed_m,*blank_m;
  image *ready7_i,*readya_i,*readyb_i,*readyc_i;
  response *data; int resp; long RT;

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
  setstim(stim,&seed); 
  /*CleanupMoviePackage();exit(1);*/

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

  ready7_i=newImage();
  downloadImage(ready7_i);
  for (s=0;s<7;s++)
  {
    sprintf(junk,"%d",s+1);
    vhline(length[map[s]],(s*16),picBuf,&seed);
    drawText(junk,xcenter,(s*16)+2,0,255);
  }
  uploadImage(ready7_i);

  readya_i=newImage();
  downloadImage(readya_i);
  {
    drawText("Tones+Lines",0,0,0,255);
  }
  uploadImage(readya_i);
  
  readyb_i=newImage();
  downloadImage(readyb_i);
  {
    drawText("Lines",0,0,0,255);
  }
  uploadImage(readyb_i);


  ready_m=initMovie(1);



  stim_m=initMovie(2);
  feed_m=initMovie(1);
  blank_m=initMovie(1);
  blank=newImage();
  targ_i=newImage();
  setMovie(stim_m,0,blank,fore_time);
  setMovie(blank_m,0,blank,1);


  t=0;
  for (b=0;b<numblocks;b++)
    {
      switch (blockorder[b]){
	  case 1: setMovie(ready_m,0,readya_i,1);break;
	  case 2: setMovie(ready_m,0,readya_i,1);break;
	  case 3: setMovie(ready_m,0,readyb_i,1);break;
	  case 4: setMovie(ready_m,0,readyb_i,1);break;}
      runMovie(ready_m,UNTIL_RESPONSE,1);
      runMovie(blank_m,FULL_SHOW,0);
      for (s=0;s<trialsperblock[b];s++)
	{
	  if (stim[t]<2 || stim[t]>4)
	    {
	      delay(50000);
	      sound(freq[stim[t]]);
	      delay(50000);
	      nosound();
	      data=runMovie(blank_m,UNTIL_RESPONSE,1);
	      RT=data->x[0].rt;
	    }
	  else
	    {
	      downloadImage(targ_i);
	      clearPicBuf();
	      hline(length[map[stim[t]]],picBuf,&seed);
	      uploadImage(targ_i);
	      setMovie(stim_m,1,targ_i,1);
	      data=runMovie(stim_m,UNTIL_RESPONSE,1);
	      RT=data->x[0].rt-(msperframe*fore_time);
	     }
	  switch (data->x[0].resp){
	  case '1': resp=0;break;
	  case '2': resp=1;break;
	  case '3': resp=2;break;
	  case '4': resp=3;break;
	  case '5': resp=4;break;
	  case '6': resp=5;break;
	  case '7': resp=6;break;
	  case '@': resp=7;break;
	  default : resp=8;audio(INVALID,wrong_m);break;}
	  if (resp==stim[t]) 
	    {
	      audio(CORRECT);
	    }
	  else
	    {
	      sprintf(junk,"%d",(stim[t])+1);
	      downloadImage(targ_i);
	      clearPicBuf();
	      drawText(junk,xcenter-10,ycenter+60,0,255);
	      uploadImage(targ_i);
	      setMovie(feed_m,0,targ_i,30);
	      runMovie(feed_m,FULL_SHOW,0);
	    }
	  runMovie(blank_m,FULL_SHOW,0);
	  sprintf(outline,"sub%03d  blk%02d  trl%03d  bt%1d %02d %02d %05d\n",
		  log->subjnum,b,s,blockorder[b],stim[t],resp,RT);
	  fprintf(outfile, "%s",outline);
	  if (backup) fprintf(backfile,"%s",outline);
	  if (resp==7) {fclose(outfile);CleanupMoviePackage();
	  printf ("stopped while running by participant\n");exit(1);}
	  t++;
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


void setstim(int *stim, long *seed)
{
  int c,b,t;
  int temp[189];
  long critseed;
  
  c=0;

  for (t=0;t<189;t++)
    temp[t]=(t%3)+2;
  distribute(temp,189,seed);
  for (t=0;t<189;t++)
    {
      stim[c]=temp[t];
      c++;
    }
  for (t=0;t<189;t++)
    temp[t]=(t%7);
  distribute(temp,189,seed);
  for (t=0;t<189;t++)
    {
      stim[c]=temp[t];
      c++;
    }
  for (t=0;t<189;t++)
    temp[t]=(t%7);
  distribute(temp,189,seed);
  for (t=0;t<189;t++)
    {
      stim[c]=temp[t];
      c++;
    }
  for (t=0;t<189;t++)
    temp[t]=(t%3)+2;
  distribute(temp,189,seed);
  for (t=0;t<189;t++)
    {
      stim[c]=temp[t];
      c++;
    }



/*  for (t=0;t<numtrials;t++)
    printf("%d %d \n",t,stim[t]);*/
}
 


