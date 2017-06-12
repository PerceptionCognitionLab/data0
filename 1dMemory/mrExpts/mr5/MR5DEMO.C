#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>

#include "movieobj.h"
#include "video.h"

#include <mem.h>
#include "exptlib.h"


#define numchain 3
#define numtrials 300
#define bottomnumber 11
#define topnumber 80
#define totalnumber (topnumber-bottomnumber)
#define status STAND_ALONE
#define backup 0
#define sizeforwidgets 10

int chainlength[numchain]={18,18,18};

void drawartalph(int num, int size, unsigned char far *a);
void vline(int y1,int y2, int x, unsigned char far *b);
void hline(int x1,int x2, int y, unsigned char far *b);
void globalstartup();
int present(int j, int a, int b, int feedback, int *respnum);
void putgeolet(char *target, unsigned char far *a);
void putenglet(char *target, unsigned char far *a);
void readP1PPM(char *img, char *filename);

movie *instruct_m,*stim_m;
image *instruct_i,*blank_i,*targ_i;
response *data;

  char **geo_pixels,**eng_pixels;

main()
{
  FILE *outfile;
  char outline[120];
  char *fontlist;
  movie *mymovie,*start_m;
  image *cross,*letter,*blank,*start_i;
  int to[totalnumber],t,c,j,i,lo[numchain][30],respnum;
  int na[numchain][30],co[numchain][30],current[numchain];
  int flag[numchain],top[numchain],success;


  long seed;

  logtype *log;


  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  seed=-(131+log->subjnum/3);
  fontlist="FONTLIST";
  SetupMoviePackage(fontlist);
  makePalette(GRAYSCALE);


  geo_pixels=(char **)malloc(sizeof(char *)*chainlength[1]); 
  for (t=0;t<chainlength[1];t++)
   {
    geo_pixels[t]=(char *)malloc(sizeof(char)*(83*83));
    if (geo_pixels[t]==NULL) {CleanupMoviePackage();printf("geo badpointer");exit(1);}
   }

  readP1PPM(geo_pixels[0],"pbmpics\\an.pbm");
  readP1PPM(geo_pixels[1],"pbmpics\\ban.pbm");
  readP1PPM(geo_pixels[2],"pbmpics\\gan.pbm");
  readP1PPM(geo_pixels[3],"pbmpics\\don.pbm");
  readP1PPM(geo_pixels[4],"pbmpics\\en.pbm");
  readP1PPM(geo_pixels[5],"pbmpics\\vin.pbm");
  readP1PPM(geo_pixels[6],"pbmpics\\tan.pbm");
  readP1PPM(geo_pixels[7],"pbmpics\\in.pbm");
  readP1PPM(geo_pixels[8],"pbmpics\\kan.pbm");
  readP1PPM(geo_pixels[9],"pbmpics\\nar.pbm");
  readP1PPM(geo_pixels[10],"pbmpics\\tar.pbm");
  readP1PPM(geo_pixels[11],"pbmpics\\un.pbm");
  readP1PPM(geo_pixels[12],"pbmpics\\par.pbm");
  readP1PPM(geo_pixels[13],"pbmpics\\kar.pbm");
  readP1PPM(geo_pixels[14],"pbmpics\\ghan.pbm");
  readP1PPM(geo_pixels[15],"pbmpics\\tsil.pbm");
  readP1PPM(geo_pixels[16],"pbmpics\\khani.pbm");
  readP1PPM(geo_pixels[17],"pbmpics\\jan.pbm");


  eng_pixels=(char **)malloc(sizeof(char *)*chainlength[0]); 
  for (t=0;t<chainlength[0];t++)
   {
    eng_pixels[t]=(char *)malloc(sizeof(char)*(83*83));
    if (eng_pixels[t]==NULL) {CleanupMoviePackage();printf("eng badpointer %d",t);exit(1);}
   }



  readP1PPM(eng_pixels[0],"pbmpics\\a.pbm");
  readP1PPM(eng_pixels[1],"pbmpics\\b.pbm");
  readP1PPM(eng_pixels[2],"pbmpics\\c.pbm");
  readP1PPM(eng_pixels[3],"pbmpics\\d.pbm");
  readP1PPM(eng_pixels[4],"pbmpics\\e.pbm");
  readP1PPM(eng_pixels[5],"pbmpics\\f.pbm");
  readP1PPM(eng_pixels[6],"pbmpics\\g.pbm");
  readP1PPM(eng_pixels[7],"pbmpics\\h.pbm");
  readP1PPM(eng_pixels[8],"pbmpics\\i.pbm");
  readP1PPM(eng_pixels[9],"pbmpics\\j.pbm");
  readP1PPM(eng_pixels[10],"pbmpics\\p.pbm");
  readP1PPM(eng_pixels[11],"pbmpics\\q.pbm");
  readP1PPM(eng_pixels[12],"pbmpics\\s.pbm");
  readP1PPM(eng_pixels[13],"pbmpics\\t.pbm");
  readP1PPM(eng_pixels[14],"pbmpics\\u.pbm");
  readP1PPM(eng_pixels[15],"pbmpics\\x.pbm");
  readP1PPM(eng_pixels[16],"pbmpics\\y.pbm");
  readP1PPM(eng_pixels[17],"pbmpics\\z.pbm");



  globalstartup();


 for (t=bottomnumber;t<topnumber;t++)
    to[t-bottomnumber]=t;
  distribute(to,totalnumber,&seed);
  c=0;
  for (j=0;j<numchain;j++)
    {
      for (i=0;i<chainlength[j];i++)
	{
	  na[(j+log->subjnum)%numchain][i]=to[c];
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
  drawText("START!!!!",xcenter,ycenter,0,255);
  uploadImage(start_i);
  setMovie(start_m,0,start_i,1);

  runMovie(start_m,UNTIL_RESPONSE,1);
  for (j=0;j<numchain;j++)
    {
      present(j,co[j][0],na[j][0],2,&respnum);
      flag[j]=0;
      current[j]=0;
      top[j]=0;
    }

  for (t=0;t<numtrials;t++)
    {
      j=t%numchain;      
      success=present(j,co[j][lo[j][current[j]]],na[j][lo[j][current[j]]],0,&respnum);
      sprintf(outline,"%02d %03d %1d %02d %02d %02d %1d %02d %02d",log->subjnum,t,j,
	co[j][lo[j][current[j]]],na[j][lo[j][current[j]]],lo[j][current[j]],success,top[j],respnum);
      outfile=fopen(log->outfn,"at");
      fprintf(outfile,"%s",outline);
      fprintf(outfile,"\n");
      fclose(outfile);
      
      if (success==0) 
	{
	  flag[j]=1;
	  present(j,co[j][lo[j][current[j]]],na[j][lo[j][current[j]]],1,&respnum);
	}
      current[j]++;
      /*printf("internal current=%d top=%d flag=%d\n",current[j],top[j],flag[j]);*/
      if (current[j]>top[j])
	{
	  if (flag[j]==0)
	    {
	      top[j]++;
	      present(j,co[j][lo[j][top[j]]],na[j][lo[j][top[j]]],2,&respnum);
	    }
	  distribute(lo[j],top[j]+1,&seed);
	  current[j]=0;
	  flag[j]=0;
	}
    }
  thankyou();
  CleanupMoviePackage();
  printf("%d %d %d\n",top[0],top[1],top[2]);
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

int present(int j, int a, int b, int feedback, int *respnum)
{

  char let[2],junk[3],respstring[3];
  
  let[1]=0;
  downloadImage(instruct_i);
  clearPicBuf();
  if (feedback==0)
    drawText("Ready",xcenter-40,ycenter,0,255);
  if (feedback==2)
    drawText("New Item",xcenter-40,ycenter,0,255);
  uploadImage(instruct_i);
  setMovie(instruct_m,0,instruct_i,24);
  if (feedback!=1) runMovie(instruct_m,UNTIL_RESPONSE,1);
  
  downloadImage(targ_i);
  clearPicBuf();
  if (j==0) 
    {
      putenglet(eng_pixels[a],picBuf);
    }
  if (j==1)
    putgeolet(geo_pixels[a],picBuf);
  if (j==2)
    /* drawartalph(a,sizeforwidgets,picBuf);*/
    hline(xcenter-(5*(a+1)),xcenter+(5*(a+1)),ycenter,picBuf);

  if (feedback>0)
    {
      sprintf(junk,"%d",b);
      drawText(junk,xcenter-10,ycenter+60,0,255);
    }
  if (feedback==1)
    drawText("Correct Answer:",xcenter-50,ycenter-70,0,255);
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
  else runMovie(stim_m,FULL_SHOW,1);
}

void drawartalph(int num, int size, unsigned char far *a)
{
  int aspect,vert,hor;
  int x,x0,x1,y,y0,y1;

  aspect=num/9;
  vert=(num%9)/3;
  hor=num%3;
  if (aspect==1) aspect=2;
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





void readP1PPM(char *img, char *filename)
{

  FILE *infile;
  char junk[80];
  int x,y,y1,x1,tempint;

  /*printf("scanning with filename %s\n",filename);*/
  infile=fopen(filename,"rt");
  fgets(junk,80,infile);
  fgets(junk,80,infile);
  fgets(junk,80,infile);
  fscanf(infile,"%d %d",&x,&y);
  /*printf("%d %d\n",x,y);*/
  for (y1=0;y1<y;y1++)
   for (x1=0;x1<x;x1++)
   {
    fscanf(infile,"%d",&tempint);
    img[y1*x+x1]=tempint;
   }
  fclose(infile);
}

void putgeolet(char *target, unsigned char far *a)
{
  int x,y;
  
  for (y=0;y<83;y++)
    for (x=0;x<83;x++)
      if (target[y*83+x]==1) a[o((118+x),(58+y))]=255;
}

void putenglet(char *target, unsigned char far *a)
{
  int x,y;
  
  for (y=0;y<82;y++)
    for (x=0;x<82;x++)
      if (target[y*82+x]==1) a[o((118+x),(58+y))]=255;
}

