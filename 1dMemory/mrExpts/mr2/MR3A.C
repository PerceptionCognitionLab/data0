#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>

#include "movieobj.h"
#include "video.h"

#include <mem.h>
#include "exptlib.h"


#define numchain 4
#define bottomnumber 11
#define topnumber 90
#define totalnumber (topnumber-bottomnumber)
#define status STAND_ALONE
#define backup 0

int chainlength[numchain]={26,27,15,5};

void drawartalph(int num, int size, unsigned char far *a);
void vline(int y1,int y2, int x, unsigned char far *b);
void hline(int x1,int x2, int y, unsigned char far *b);
void globalstartup();
int present(int j, int a, int b, int feedback, int *respnum, long *seed);

movie *instruct_m,*stim_m,*delay_m;
image *instruct_i,*blank_i,*targ_i;
response *data;
main()
{
  FILE *outfile;
  char outline[120];
  char *fontlist;
  movie *mymovie,*start_m;
  image *cross,*letter,*blank,*start_i;
  int to[totalnumber],t,c,j,j1,i,s,lo[numchain][30],respnum;
  int na[numchain][30],co[numchain][30],current[numchain];
  int flag[numchain],top[numchain],success;

  long seed=-123;

  logtype *log;


  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  fontlist=getenv("FONTLIST");
  SetupMoviePackage(fontlist);
  makePalette(GRAYSCALE);

  globalstartup();

 for (t=bottomnumber;t<topnumber;t++)
    to[t-bottomnumber]=t;
  distribute(to,totalnumber,&seed);
  c=0;
  for (j=0;j<numchain;j++)
    {
      for (i=0;i<chainlength[j];i++)
	{
	  na[j][i]=to[c];
	  co[j][i]=i;
	  c++;
	}
      distribute(co[j],chainlength[j],&seed);
    }
  
  for (j=0;j<numchain;j++)
    for (t=0;t<30;t++)
      lo[j][t]=t;


  start_m=initMovie(1);
  start_i=newImage();
  downloadImage(start_i);
  drawText("NEW MATERIALS",xcenter,ycenter,0,255);
  uploadImage(start_i);
  setMovie(start_m,0,start_i,1);

  delay_m=initMovie(1);
  setMovie(delay_m,0,blank_i,100);


  for (j1=0;j1<numchain;j1++)
    {
      j=(j1+log->subjnum)%numchain;
      downloadImage(start_i);
      if (j==0) drawText("LETTERS",xcenter,ycenter,0,255);
      if (j==1) drawText("FIGURES",xcenter,ycenter,0,255);
      if (j==2) drawText("LINES",xcenter,ycenter,0,255);
      if (j==4) drawText("PUNCTUATION",xcenter,ycenter,0,255);
      runMovie(start_m,UNTIL_RESPONSE,1);
      present(j,co[j][0],na[j][0],2,&respnum,&seed);
      flag[j]=0;
      current[j]=0;
      top[j]=0;
   
      for (s=0;s<135;s++)
	{  
	  t=j*100+s;
          success=present(j,co[j][lo[j][current[j]]],na[j][lo[j][current[j]]],0,&respnum,&seed);
	  sprintf(outline,"%02d %03d %1d %02d %02d %02d %1d %02d %02d",log->subjnum,t,j,
		  co[j][lo[j][current[j]]],na[j][lo[j][current[j]]],lo[j][current[j]],success,top[j],respnum);
	  outfile=fopen(log->outfn,"at");
	  fprintf(outfile,"%s",outline);
	  fprintf(outfile,"\n");
	  fclose(outfile);
	  
	  if (success==0) 
	    {
	      flag[j]=1;
              present(j,co[j][lo[j][current[j]]],na[j][lo[j][current[j]]],1,&respnum,&seed);
	    }
	  current[j]++;
	  /*printf("internal current=%d top=%d flag=%d\n",current[j],top[j],flag[j]);*/
	  if (current[j]>top[j])
	    {
	      if (flag[j]==0)
		{
		  top[j]++;
                  present(j,co[j][lo[j][top[j]]],na[j][lo[j][top[j]]],2,&respnum,&seed);
		}
	      distribute(lo[j],top[j]+1,&seed);
	      current[j]=0;
	      flag[j]=0;
	    }
	}
    }
  thankyou();
  CleanupMoviePackage();
  printf("%d %d %d %d\n",top[0],top[1],top[2],top[3]);
}

void globalstartup()
{
  instruct_m=initMovie(1);
  instruct_i=newImage();
  stim_m=initMovie(2);
  blank_i=newImage();
  targ_i=newImage();
  setMovie(stim_m,0,blank_i,30);
}

int present(int j, int a, int b, int feedback, int *respnum,long *seed)
{

  char let[2],junk[3],respstring[3];
  int backcountnumber;
  char backcountstring[60];


  backcountnumber=randint(500,999,seed);
  sprintf(backcountstring,"Count Backwards From %d",backcountnumber);
  let[1]=0;
  downloadImage(instruct_i);
  clearPicBuf();
  if (feedback==0)
    drawText("Ready",xcenter-40,ycenter,0,255);
  if (feedback==2)
   {
    drawText("New Item",xcenter-40,ycenter,0,255);
    drawText(backcountstring,xcenter-80,ycenter+20,0,255);
    }
  if (feedback==1)
   {
    drawText("WRONG. The correct answer was",xcenter-80,ycenter,0,255);
    drawText(backcountstring,xcenter-80,ycenter+20,0,255);
    }

  uploadImage(instruct_i);
  setMovie(instruct_m,0,instruct_i,24);
  if (feedback>0) runMovie(instruct_m,UNTIL_RESPONSE,1);

 

  downloadImage(targ_i);
  clearPicBuf();
  if (j==0) 
    {
      let[0]=65+a;
      drawText(let,xcenter,ycenter,0,255);
    }
  if (j==1)
    drawartalph(a,3,picBuf);
  if (j==2)
    hline(xcenter-(5*(a+1)),xcenter+(5*(a+1)),ycenter,picBuf);
  if (j==3)
    {
  if (feedback>0)
    {
      sprintf(junk,"%d",b);
      drawText(junk,xcenter-10,ycenter+40,0,255);
    }
  /*if (feedback==1)
    drawText("Correct Answer:",xcenter-50,ycenter-50,0,255);*/
  uploadImage(targ_i);
  setMovie(stim_m,1,targ_i,60);
  if (feedback==0)
    {
      data=runMovie(stim_m,FULL_SHOW,2);
      if (data->x[0].resp=='v') {CleanupMoviePackage();exit(1);}
      sprintf(respstring,"%c%c",data->x[0].resp,data->x[1].resp);
      *respnum=atoi(respstring);
      if (*respnum==b) audio(CORRECT);
      /*else audio (ERROR);*/
      if (*respnum==b) return 1;
      else return 0;
    }
  else
   {
   sound(500);
   delay(5000);
   nosound();
   runMovie(delay_m,FULL_SHOW,0);
   runMovie(stim_m,FULL_SHOW,0);
   runMovie(delay_m,FULL_SHOW,0);
   sound(500);
   delay(5000);
   nosound();

   }
}

void drawartalph(int num, int size, unsigned char far *a)
{
  int aspect,vert,hor;
  int x,x0,x1,y,y0,y1;

  aspect=num/9;
  vert=(num%9)/3;
  hor=num%3;
  /*horizontal segment*/
  y=ycenter+(hor-1)*(aspect+1)*size;
  x0=xcenter-(3-aspect)*size;
  x1=xcenter+(3-aspect)*size;
  hline(x0,x1,y,a);
  /*vertical segment*/
  x=xcenter+(vert-1)*(3-aspect)*size;
  y0=ycenter-(aspect+1)*size;
  y1=ycenter+(aspect+1)*size;
  vline(y0,y1,x,a);
  /*a[o(xcenter,ycenter)]=127;*/
}

  
void vline(int y1,int y2, int x, unsigned char far *b)
{
  int q;
  for (q=y1;q<=y2;q++) b[o(x,q)]=255;
}

void hline(int x1,int x2, int y, unsigned char far *b)
{
  int q;
  for (q=x1;q<=x2;q++) b[o(q,y)]=255;
}
