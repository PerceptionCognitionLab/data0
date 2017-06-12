#include <stdio.h>       
#include <stdlib.h>
#include <string.h> /*for memcpy command*/
#include "movieobj.h"
#include "video.h"
#include <mem.h>
#include "exptlib.h"
#include <math.h>
#include <time.h>

#define status STAND_ALONE
#define feedbacklength 42
#define WRONG 0
#define RIGHT 1
#define numtrialsperblock 42
#define stimperblock 6
#define numblocks 18
#define numlength 12
#define maxmovex 6
#define maxmovey 6
#define breaklength 120
#define fore_time 30

void hline(int x1,int x2, int y, unsigned char far *b);

int main(){
 int i=0,j=0,k=0,movepixelx=0,movepixely=0,l,cor;
 int stimuli[numtrialsperblock];
// int blocktypem={0,1};
 int length[numlength]={14,21,31,44,59,79,102,130,164,202,248,300};
  logtype *log;
  FILE *outfile;
  char *fontlist;
  char outline[80];
  long seed;

  image *ready_i,*wrong_i,*blank, *break_i, *stim_i,*feedback_i,*fast_i;
  movie *wrong_m,*ready_m,*stim_m,*break_m,*feed_m,*feedback_m,*fast_m;
  response *data; int resp; long RT;
  char feedback[2];
  int blocktype;
  fontlist=getenv("FONTLIST");
  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  SetupMoviePackage(fontlist);
  seed=log->seed;
  outfile=fopen(log->outfn,"wt");
  

   makePalette(GRAYSCALE);

    blank=newImage();
  fast_i=newImage();
  downloadImage(fast_i);
  drawText("Too fast!",xcenter-20,ycenter,0,255);
  uploadImage(fast_i);
  fast_m=initMovie(1);
  setMovie(fast_m,0,fast_i,120);
  
  /*READY movie*/
    ready_i=newImage();
  downloadImage(ready_i);
  drawText("ready?",0,0,0,253);
  uploadImage(ready_i);
  ready_m=initMovie(1);
  setMovie(ready_m,0,ready_i,1);

  /*WRONG movie*/
  wrong_i=newImage();
  downloadImage(wrong_i);
  drawText("invalid key",0,0,0,253);
  uploadImage(wrong_i);
  wrong_m=initMovie(1);
  setMovie(wrong_m,0,wrong_i,1);
 

  /*break movie*/
  break_i=newImage();
  downloadImage(break_i);
  drawText("Break - please complete worksheets",0,0,0,255);
  uploadImage(break_i);
  break_m=initMovie(1);
  setMovie(break_m,0,break_i,breaklength*60);
 
 feedback_m=initMovie(1);
 feedback_i=newImage();
 stim_m=initMovie(2);
 blank=newImage();
 stim_i=newImage();
 setMovie(stim_m,0,blank,fore_time);

 //start running trials
 
 for (i=0;i<numblocks;i++){
 runMovie(ready_m,UNTIL_RESPONSE,1);
 
    if ((log->subjnum)%2){
	    blocktype=((i+1)/2)%2;
    }else{
	    blocktype=((i+3)/2)%2;
    }
  
    for (j=0;j<numtrialsperblock;j++)
	stimuli[j]=(j%stimperblock)+(blocktype*6);
   distribute(stimuli,numtrialsperblock,&seed);
 

    for (k=0;k<numtrialsperblock;k++){
	movepixelx=randint(-maxmovex,maxmovex,&seed);
	movepixely=randint(-maxmovey,maxmovey,&seed);
	l=length[stimuli[k]];

  downloadImage(stim_i);
  clearPicBuf();
  hline(xcenter-(l/2)+movepixelx,xcenter-(l/2)+l+movepixelx,ycenter+movepixely,picBuf);
  uploadImage(stim_i);
  setMovie(stim_m,1,stim_i,1);
  sprintf(feedback,"  ");
  sprintf(feedback,"%i",stimuli[k]-(blocktype*6)+1);  

  downloadImage(feedback_i);
  clearPicBuf();
  drawText(feedback,xcenter,ycenter+15,0,255);
  uploadImage(feedback_i);
  setMovie(feedback_m,0,feedback_i,feedbacklength);

	data=runMovie(stim_m, UNTIL_RESPONSE,1);

	  switch (data->x[0].resp){
	  case '1': resp=0;break;
	  case '2': resp=1;break;
	  case '3': resp=2;break;
	  case '4': resp=3;break;
	  case '5': resp=4;break;
	  case '6': resp=5;break;
	  case '@': resp=10;break;
	  default : resp=11;audio(INVALID, wrong_m);break;}
	  RT=data->x[0].rt-(msperframe*fore_time);
	  
	  if ((resp+(blocktype*6))==stimuli[k])
	    {
	    audio(CORRECT);
	    cor=1;
	   }
	 else
	    {
	      /*audio(ERROR);*/
	      cor=0;
	      runMovie(feedback_m,FULL_SHOW,0);
	    }  
	  if (RT<300){
		audio(INVALID, fast_m);
	  }
	  if (resp==10) {fclose(outfile);CleanupMoviePackage();
	  printf ("stopped while running by participant\n");exit(1);}

sprintf(outline, "sub%03d blk%i trl%03d btp%i crs%i stim%i rsp%i %i %05d\n",
 log->subjnum,i,(i*numtrialsperblock)+k,blocktype,stimuli[k]-(blocktype*6),stimuli[k],resp,cor,RT);
fprintf(outfile,"%s",outline);  
}
if (i<numblocks-1){
runMovie(break_m,FULL_SHOW,0);
sound(400);
delay(5000);
nosound();
}


}
 //cleanup
 fclose(outfile);
 thankyou();
 CleanupMoviePackage();
 return 0;
}



void hline(int x1,int x2, int y, unsigned char far *b)
{
  int q;
  for (q=x1;q<=x2;q++) b[o(q,y)]=255;
}

void makebreak(int secs, movie *movie_m){
int begin,end;
begin=(int)(time(NULL));
end=begin+secs;
while((int)(time(NULL))<end){
runMovie(movie_m,FULL_SHOW,0);
}
}



