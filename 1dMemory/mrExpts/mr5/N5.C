/*greater|less than 5*/
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

/*factors*/
/* trial characteristics
size of square: 4 levels, see size[]
location on screen: 4 places see x[] and y[]*/

#define numnum 6

/* times */

#define blank_time 60
/*places*/



int num[numnum]={2,3,4,6,7,8};


main()
{

  logtype *log,*backfile;
  FILE *outfile;
  int trial[numtrials],let,cor;
  long seed;
  int b,s,t,i;
  char *fontlist;
  char backstr[40],outline[80],showlet[2];


  image *blank_i,*ready_i,*welcome_i,*wrong_i,*feedback_i, *targ_i;
  movie *welcome_m,*ready_m,*wrong_m,*feedback_m;
  movie *stim_m;
  response *data; int resp; long RT;



  showlet[1]=0;

  /*initialize movie package*/


  fontlist=getenv("FONTLIST");
  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  seed=log->seed;
  outfile=fopen(log->outfn,"wt");
  if (backup)
    {
     sprintf(backstr,"c:\\expbak\\%s",log->outfn);
     backfile=fopen(backstr,"wt");
    }


  for (t=0;t<numtrials;t++)
    trial[t]=t%(numnum);
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

  stim_m=initMovie(2);
  setMovie(stim_m,0,blank_i,blank_time);

  feedback_i=newImage();
  feedback_m=initMovie(1);

  targ_i=newImage();

  /* big loop */
  for (b=0;b<numblocks;b++)
    {
      runMovie(ready_m,UNTIL_RESPONSE,1);
      for (s=0;s<numtrialsperblock;s++)
	{
	  t=b*(numtrialsperblock)+s;
	  /*set variable frames here*/
	  downloadImage(targ_i);
	  clearPicBuf();
	  sprintf(showlet,"%d",num[trial[t]]);
	  drawText(showlet,xcenter,ycenter,0,255);
	  uploadImage(targ_i);
	  setMovie(stim_m,1,targ_i,1);
	  data=runMovie(stim_m, UNTIL_RESPONSE,1);

	  switch (data->x[0].resp){
	  case 'z': resp=0;break;
	  case 'Z': resp=0;break;
          case '/': resp=1;break;
          case '?': resp=1;break;
	  case '@': resp=9;break;
	  default : resp=8;audio(INVALID,wrong_m);break;}
	  RT=data->x[0].rt-(msperframe*blank_time);
          if (resp==trial[t]/(numnum/2)) {audio(CORRECT); cor=0;}
          else {audio(ERROR); cor=1;}
   
          sprintf(outline,"%03d  %02d  %03d  %d  %1d %05ld %d\n",
                  log->subjnum,b,s,trial[t],resp,RT,cor);
	  fprintf(outfile, "%s",outline);
	  if (backup) fprintf(backfile,"%s",outline);
          if (resp==9) {fclose(outfile);CleanupMoviePackage();
	  printf ("stopped while running by participant\n");exit(1);}
	}
    }
  fclose(outfile);
  if (backup) fclose(backfile);
  thankyou();
  CleanupMoviePackage();
}
