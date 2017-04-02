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
#define numtrialsperblock 60
#define stimperblock 12
#define numblocks 9
#define numlength 16
#define breaklength 90
#define fore_time 30
#define totaltrials numtrialsperblock*numblocks
#define gaborside 64
#define Lm0 127
#define fc0 1.5
#define sd0 .5   //never set this to 0
#define PI 3.1415926535892
#define radius0 1

void hline(int x1,int x2, int y, unsigned char far *b);
void makegabor(float *x, float Lm, float Cp, float fc, float sd, float radius,unsigned char far *b);
float gabor(float x, float y, float Lm, float Cp, float fc, float sd);
unsigned char gabor2(float x, float y, float Lm, float Cp, float fc, float sd);
void picbufout(unsigned char far *b, FILE *out);
unsigned char gabor4(float x, float y, float Lm, float Cp, float fc, float sd,float radius);
unsigned char redgreen(float propred);
void makeredgreen(float propred, unsigned char far *b);

int sign(float x);

int main(){
 int i=0,k=0,cor;
 int day=0;
 int stimuli[totaltrials];

// int blocktypem={0,1};
  double length[numlength]={.0625,.125,.1875,0.2500,0.3125,0.3750,0.4375,0.5000,0.5625,0.6250,0.6875,0.7500,0.8125,0.8750,0.9375,1.0000};
  int lengthsa[stimperblock]={2,3,4,5,6,7,8,9,10,11,12,13};
  int lengthsb[stimperblock]={0,1,4,5,6,7,8,9,10,11,14,15};
//  int lengthsc[stimperblock]={0,1,2,3,6,7,8,9,12,13,14,15};
  int mylengths[stimperblock];
  int stimset,cond;
  logtype *log;
  FILE *outfile;
  float l;
  char *fontlist;
  char outline[80];
  long seed;
  float x[gaborside];
  image *ready_i,*wrong_i,*blank, *break_i, *stim_i,*feedback_i,*fast_i;
  movie *wrong_m,*ready_m,*stim_m,*break_m,*feed_m,*feedback_m,*fast_m;
  response *data; int resp; long RT;
  char feedback[2];
  int blocktype;
  FILE *dump; 

  fontlist=getenv("FONTLIST");
  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  printf("What day number is this run (first run=0)?\n");
  fscanf(stdin,"%d",&day);
  SetupMoviePackage(fontlist);
  seed=log->seed;
  outfile=fopen(log->outfn,"wt");
  dump=fopen("dump.pbm","wt"); 

  for (i=0;i<gaborside;i++){
    x[i]=-5+(.05*i);

  }
   
  makePalette(GRAYSCALE);
   Palette[1].r=0;Palette[1].g=255;Palette[1].b=0;
  /*green*/
   Palette[254].r=255;Palette[254].g=0;Palette[254].b=0;
  /*red*/

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
  drawText("Break",0,0,0,255);
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
 cond=(log->subjnum)%2;
 stimset=(((6*cond)+day)/3)%2;
 for (i=0;i<totaltrials;i++){ 
	stimuli[i]=i%stimperblock;        
 }
 distribute(stimuli,totaltrials,&seed);
 
 for (i=0;i<numblocks;i++){
 runMovie(ready_m,UNTIL_RESPONSE,1);

 for (k=0;k<numtrialsperblock;k++){
	if (stimset==0){
		l=length[lengthsa[stimuli[(i*numtrialsperblock)+k]]];
	}else{
		l=length[lengthsb[stimuli[(i*numtrialsperblock)+k]]];
	}
  
  downloadImage(stim_i);
  clearPicBuf();
  /*  hline(xcenter-(l/2)+movepixelx,xcenter-(l/2)+l+movepixelx,ycenter+movepixely,picBuf);*/
//  makegabor(x, Lm0, l, fc0, sd0, radius0 ,picBuf);
//picbufout(picBuf,dump);
makeredgreen(l,picBuf);

  uploadImage(stim_i);
  setMovie(stim_m,1,stim_i,1);
  sprintf(feedback,"  ");
  sprintf(feedback,"%i",stimuli[(i*numtrialsperblock)+k]+1);  

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
	  case '7': resp=6;break;
	  case '8': resp=7;break;
	  case '9': resp=8;break;
	  case '0': resp=9;break;
	  case '-': resp=10;break;
	  case '=': resp=11;break;
	  case '@': resp=12;break;
	  default : resp=13;audio(INVALID, wrong_m);break;}
	  RT=data->x[0].rt-(msperframe*fore_time);
	  
	  if (resp==stimuli[(i*numtrialsperblock)+k])
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
	  if (resp==12) {fclose(outfile);CleanupMoviePackage();
	  printf ("stopped while running by participant\n");exit(1);}

sprintf(outline, "sub%03d day%i blk%i trl%03d set%i stim%i rsp%i %i %05d\n",
 log->subjnum,day,i,(i*numtrialsperblock)+k,stimset,stimuli[(i*numtrialsperblock)+k],resp,cor,RT);
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


void makegabor(float *x, float Lm, float Cp, float fc, float sd, float radius,unsigned char far *b)
{
	int i,j,x0,y0;
	x0=xcenter-(gaborside/2)+1;
	y0=ycenter-(gaborside/2)+1;
	for (i=0;i<gaborside;i++){
		for (j=0;j<gaborside;j++){
			b[o(j+x0,i+y0)-1]=gabor4(x[j],x[i],Lm,Cp,fc,sd,radius);
		}
	}
}

float gabor(float x, float y, float Lm, float Cp, float fc, float sd)
{
  float gab; 
  gab=Lm*(1+(Cp*cos(2*PI*x*fc)*exp(-.5*pow(x/sd,2)-.5*pow(y/sd,2))));
  return (gab);
}
   
unsigned char gabor2(float x, float y, float Lm, float Cp, float fc, float sd)
{
  float gab; 
  gab=Lm*(1+(Cp*cos(2*PI*x*fc)*exp(-.5*((x/sd)*(x/sd))-.5*((y/sd)*(y/sd)))))+((rand()/(RAND_MAX+1))*3)-1;
  return (gab);
}

unsigned char gabor4(float x, float y, float Lm, float Cp, float fc, float sd, float radius)
{
  float gpart=0,x1=0,y1=0,gab;
  int signx=sign(x);
  int signy=sign(y);
  if((sqrt(pow(x,2)+pow(y,2)))<radius){
    gpart=1;
  }else{
    if(x!=0){
	x1=signx*radius*cos(atan2(fabs(y),fabs(x)));
	y1=signy*radius*sin(atan2(fabs(y),fabs(x)));
    }else{
	x1=0;
	y1=radius;
    }
    gpart=exp(-.5*pow((x-x1)/sd,2)-.5*pow((y-y1)/sd,2));
  }
  gab=Lm*(1+(Cp*cos(2*PI*x*fc)*gpart));
  return((int)(gab));
}


void picbufout(unsigned char far *b, FILE *out)
{
int i,j;
fprintf(out,"P2\n# picbuf dump\n320 200\n255\n");
for (i=0;i<200;i++){
	for(j=0;j<320;j++){
		fprintf(out,"%i ",(int)(b[o(j,i)]));
	}
	fprintf(out,"\n");
}
//CleanupMoviePackage();
//exit(0);
}

int sign(float x){
int sign=0;
if (x<0){sign=-1;}
if (x==0){sign=0;}
if (x>0){sign=1;}
return sign;
}

void makeredgreen(float propred, unsigned char far *b)
{
	int i,j,x0,y0;
	x0=xcenter-(gaborside/2)+1;
	y0=ycenter-(gaborside/2)+1;
	for (i=0;i<gaborside;i++){
		for (j=0;j<gaborside;j++){
			b[o(j+x0,i+y0)-1]=redgreen(propred);
		}
	}

}

unsigned char redgreen(float propred){
float f;
int ret;
f=(1.*rand()/RAND_MAX);
if (f>propred){
	ret=254;
}else{
	ret=1;
}
return((unsigned char)(ret));
}
