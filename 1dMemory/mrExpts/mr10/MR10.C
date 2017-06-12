#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>

#include "movieobj.h"
#include "video.h"

#include <mem.h>
#include "exptlib.h"


#define numchain 5
#define bottomnumber 11
#define topnumber 80
#define totalnumber (topnumber-bottomnumber)
#define stimperchain 4

#define status STAND_ALONE
#define backup 0


int present(int chain,int stim,int resp);
int present_stim(int j, int a, int b, int feedback);
void readP1PPM(char *img, char *filename);
void putgeolet(char *target, unsigned char far *a);
void putenglet(char *target, unsigned char far *a);
void hline(int x1,int x2, int y, unsigned char far *b);
void contrastbox(int contrast,unsigned char far *b);
void putshape(int a,unsigned char far *b);

char **geo_pixels,**eng_pixels;
image *targ_i,*blank_i;
movie *stim_m;
logtype *mylog;

int chainorder[4][numchain]={{4,0,1,2,3},{4,3,2,1,0},{4,1,3,0,2},{4,2,0,3,1}};
int contrastdiffs[stimperchain]={5,15,45,100};
main()
{
  int t,c,resp_temp[100],countbal,ch,flag;
  int resp[numchain][stimperchain],order[stimperchain];
  int chain,stim,cycle;
  char *fontlist;      

  image *newchain_i; 
  movie *newchain_m;
  long seed;

  mylog=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,mylog);
  seed=-(1133+(mylog->subjnum%100)/4);
  printf("REAL SEED %ld\n",seed);
  getchar();
  countbal=mylog->subjnum%4;
  fontlist=getenv("FONTLIST");
  SetupMoviePackage(fontlist);
  makePalette(GRAYSCALE);


  geo_pixels=(char **)malloc(sizeof(char *)*stimperchain); 
  for (t=0;t<stimperchain;t++)
   {
    geo_pixels[t]=(char *)malloc(sizeof(char)*(83*83));
    if (geo_pixels[t]==NULL) {CleanupMoviePackage();printf("geo badpointer");exit(1);}
   }
  readP1PPM(geo_pixels[0],"..\\pbmpics\\an.pbm");
  readP1PPM(geo_pixels[1],"..\\pbmpics\\ban.pbm");
  readP1PPM(geo_pixels[2],"..\\pbmpics\\gan.pbm");
  readP1PPM(geo_pixels[3],"..\\pbmpics\\don.pbm");

  eng_pixels=(char **)malloc(sizeof(char *)*stimperchain); 
  for (t=0;t<stimperchain;t++)
   {
    eng_pixels[t]=(char *)malloc(sizeof(char)*(83*83));
    if (eng_pixels[t]==NULL) {CleanupMoviePackage();printf("eng badpointer %d",t);exit(1);}
   }

  readP1PPM(eng_pixels[0],"..\\pbmpics\\d.pbm");
  readP1PPM(eng_pixels[1],"..\\pbmpics\\g.pbm");
  readP1PPM(eng_pixels[2],"..\\pbmpics\\q.pbm");
  readP1PPM(eng_pixels[3],"..\\pbmpics\\u.pbm");

  stim_m=initMovie(2);
  blank_i=newImage();
  targ_i=newImage();
  setMovie(stim_m,0,blank_i,30);
  newchain_m=initMovie(1);
  newchain_i=newImage();
  downloadImage(newchain_i);
  clearPicBuf();
  drawText("New Set",xcenter,ycenter,0,255);
  uploadImage(newchain_i);
  setMovie(newchain_m,0,newchain_i,1);


  /* assign response terms*/
  for (t=bottomnumber;t<topnumber;t++)
    resp_temp[t-bottomnumber]=t;
  distribute(resp_temp,totalnumber,&seed);


  c=0;
  for (chain=0;chain<numchain;chain++)
    for (stim=0;stim<stimperchain;stim++)
      {
        resp[chainorder[countbal][chain]][stim]=resp_temp[c];
	c++;
      }



  for (stim=0;stim<stimperchain;stim++)
    order[stim]=stim;

  for (ch=0;ch<numchain;ch++)
    {
      chain=chainorder[countbal][ch];
      runMovie(newchain_m,UNTIL_RESPONSE,1);
      do
	{
	  cycle=0;
	  stim=0;
	  distribute(order,stimperchain,&seed);
          flag=1;
          while (flag==1 && cycle<2)
	    {
              flag=present(chain,order[stim],resp[chain][order[stim]]);
	      stim++;
	      if (stim==stimperchain)
		{
		  stim=0;
		  cycle++;
		  distribute(order,stimperchain,&seed);
		}
	      
	    }	
	} while (cycle<2);
    } 
  thankyou();
  CleanupMoviePackage();
}

int present(int chain,int stim,int resp)
{
  if (present_stim(chain,stim,resp,0)==1)
    return(1);
  else
    {
      present_stim(chain,stim,resp,1);
      return(0);
    }
}
	
      
      


int present_stim(int j, int a, int b, int feedback)
{
  char junk[85],respstring[3],outline[80];
  int resp,correct;
  response *data;
  FILE *outfile;
  
  downloadImage(targ_i);
  clearPicBuf();
  switch (j)
    {
    case 0: putenglet(eng_pixels[a],picBuf);break;
    case 1: putgeolet(geo_pixels[a],picBuf);break;
    case 2: hline(xcenter-(5*(a+1)),xcenter+(5*(a+1)),ycenter,picBuf); break;
    case 3:
      {
	fillPicBuf(155);
	contrastbox(a,picBuf);
      }
      break;
    case 4: putshape(a,picBuf);break;
    }

  if (feedback) 
    {
      sprintf(junk,"%d",b);
      drawText(junk,xcenter-10,ycenter+60,0,255);
      uploadImage(targ_i);
      setMovie(stim_m,1,targ_i,60);
      runMovie(stim_m,FULL_SHOW,1);
      return 0;
    }
  else
    {
      uploadImage(targ_i);
      setMovie(stim_m,1,targ_i,60);
      data=runMovie(stim_m,FULL_SHOW,2);
      if (data->x[0].resp=='v') {CleanupMoviePackage();exit(1);}
      sprintf(respstring,"%c%c",data->x[0].resp,data->x[1].resp);
      resp=atoi(respstring);
      if (resp==b) correct=1; else correct=0;
      if (correct) audio(CORRECT);
      sprintf(outline,"%04d %d %d %d %02d %d\n",
              mylog->subjnum,j,a,b,resp,correct);
      outfile=fopen(mylog->outfn,"at");
      fprintf(outfile,"%s",outline);
      fclose(outfile);
      if (correct) return 1;
      else return 0;
    }
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

  
void hline(int x1,int x2, int y, unsigned char far *b)
{
  int q;
  for (q=x1;q<=x2;q++) b[o(q,y)]=255;
}

void contrastbox(int contrast,unsigned char far *b)
{
#define grey 155
  int col[2];

  col[0]=grey-contrastdiffs[contrast];
  col[1]=grey+contrastdiffs[contrast];
  putcheckerbox(50,10,xcenter,ycenter,col,b);
}

void putshape(int a,unsigned char far *b)
{
  switch (a)
    {
    case 0: putsquare3(15,xcenter,ycenter,255,b);break;
    case 1: putcircle5(10,xcenter,ycenter,255,b);break;
    case 2: puttriangle3(10,xcenter,ycenter,255,b);break;
    case 3: putcross(10,xcenter,ycenter,255,b);break;
    }
}
