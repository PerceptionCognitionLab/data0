/*absolute id*/

#include <stdio.h>
#include <stdlib.h>

#include "movieobj.h"
#include "video.h"

#include <mem.h>
#include "exptlib.h"

#define backup 0
#define status STAND_ALONE
/*------------------------------------*/

#define numblocks 6
#define numtrialsperblock 60
#define numtrials numblocks*numtrialsperblock


#define numlength 10
#define numperday 8

/* times */

#define blank_time 60
#define setseed -24536

void makemask(movie *mask5, image **lots, long *seed);
void hline(int x1,int x2, int y, unsigned char far *b);


int length[numlength]={37,45,55,67,81,99,121,148,181,221};
/*x_1:12 & length_5*exp(.3*x)*/
int day0[numperday]={0,1,2,3,4,5,6,7};
int day1[numperday]={2,3,4,5,6,7,8,9};

main()
{

  logtype *log,*backfile;
  FILE *outfile;
  long seed;
  int b,s,t,day,trial[numtrials],i,l;
  char *fontlist;
  char backstr[40],outline[80],showlet[10];


  image *blank_i,*ready_i,*welcome_i,*wrong_i,*feedback_i, *line_i;
  image **lots;
  movie *welcome_m,*ready_m,*wrong_m,*feedback_m;
  movie *stim_m,*mask_m;
  response *data; int resp; long RT;

  lots=(image **)malloc(5*sizeof(image *));

  /*initialize movie package*/


  fontlist=getenv("FONTLIST");
  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  seed=setseed;
  printf("Which session (0/1)\n");
  scanf("%d",&day);

  outfile=fopen(log->outfn,"wt");



  /* assign trials*/
  /*if program crashes, try allocating trial dynamically AFTER
    calling the movie package*/

  for (t=0;t<numtrials;t++)
    trial[t]=t%numperday;
  distribute(trial,numtrials,&seed);


  SetupMoviePackage(fontlist);

  /* set up pallete here*/
  makePalette(GRAYSCALE);

  blank_i=newImage();
  /*READY movie*/
  ready_i=newImage();
  downloadImage(ready_i);
  drawText("ready?",0,0,0,255);
  uploadImage(ready_i);
  ready_m=initMovie(1);
  setMovie(ready_m,0,ready_i,1);
  /*wrong movie*/
  wrong_i=newImage();
  downloadImage(wrong_i);
  drawText("invalid key",0,0,0,255);
  uploadImage(wrong_i);
  wrong_m=initMovie(1);
  setMovie(wrong_m,0,wrong_i,24);
   /*welcome movie*/
  welcome_i=newImage();
  downloadImage(welcome_i);
  drawText("WELCOME",0,0,1,255);
  uploadImage(welcome_i);
  welcome_m=initMovie(1);
  setMovie(welcome_m,0,welcome_i,1);

  line_i=newImage();
  for (i=0;i<5;i++)
    lots[i]=newImage();
  stim_m=initMovie(2);
  setMovie(stim_m,0,blank_i,blank_time);
  mask_m=initMovie(5);

  feedback_i=newImage();
  feedback_m=initMovie(1);
  
  
  
  /* big loop */
  for (b=0;b<numblocks;b++)
    {
      runMovie(ready_m,UNTIL_RESPONSE,1);
      for (s=0;s<numtrialsperblock;s++)
	{
	 showlet[2]=0;
	 showlet[3]=0;
	  t=b*(numtrialsperblock)+s;
	  if (day==0)
	  l=length[day0[trial[t]]];
	  else l=length[day1[trial[t]]];
	  /*set variable frames here*/
	  downloadImage(line_i);
	  clearPicBuf();
	  hline(xcenter-l/2,xcenter-l/2+l,ycenter,picBuf); 
	  uploadImage(line_i);
	  setMovie(stim_m,1,line_i,1);
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
	  case '@': resp=10;break;
	  default : resp=11;audio(INVALID,wrong_m);break;}
	  RT=data->x[0].rt-(msperframe*blank_time);
	  if (resp==trial[t]) audio(CORRECT);
	  else
	    {
	      downloadImage(feedback_i);
	      clearPicBuf();
	      sprintf(showlet,"%d",(trial[t])+1);
	      drawText(showlet,xcenter,ycenter,0,255);
	      uploadImage(feedback_i);
	      setMovie(feedback_m,0,feedback_i,60);
	      runMovie(feedback_m,FULL_SHOW,0);
	    }  

	 /* makemask(mask_m,lots,&seed); */
	  sprintf(outline,"%03d  %02d  %03d  %d %d %03d %02d %05d\n",
		  log->subjnum,b,s,day,trial[t],l,resp,RT);
	  fprintf(outfile, "%s",outline);
	  if (resp==10) {fclose(outfile);CleanupMoviePackage();
	  printf ("stopped while running by participant\n");exit(1);}
	}
    }
  fclose(outfile);
  thankyou();
  CleanupMoviePackage();
}

void makemask(movie *mask5, image **lots, long *seed)
{
#define numsquares 25
  int i,j,x,y,size;

  for (j=0;j<5;j++)
    {
     downloadImage(lots[j]);
     clearPicBuf();
     for (i=0;i<numsquares;i++)
      {
       x=randint(xcenter-30,xcenter+30,seed);
       y=randint(ycenter-30,ycenter+30,seed);
       size=randint(5,180,seed);
       putsquare(size,x,y,255,picBuf);
      }
    uploadImage(lots[j]);
    setMovie(mask5,j,lots[j],6);
   }
  runMovie(mask5,FULL_SHOW,0);
}


void hline(int x1,int x2, int y, unsigned char far *b)
{
  int q;
  for (q=x1;q<=x2;q++) b[o(q,y)]=255;
}

