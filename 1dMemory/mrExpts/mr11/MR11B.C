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
#define topnumber 80
#define stimperchain 5
#define stimperchaintotal 8
#define numcountbal 3
#define numsets 4

#define status STAND_ALONE
#define backup 0


int present(int chain,int stim);
int present_stim(int j, int a, int feedback);
void readP1PPM(char *img, char *filename);
void putgeolet(char *target, unsigned char far *a);
void hline(int x1,int x2, int y, unsigned char far *b);
void putshape(int a,unsigned char far *b);

char **geo_pixels,**eng_pixels;
image *targ_i,*blank_i;
movie *stim_m;
logtype *mylog;

int oldnew[2][stimperchain]={{0,1,2,4,7},{0,3,5,6,7}};
int group[numsets][numchain][2]={{{0,0},{1,0},{0,1},{0,0}},{{1,1},{0,1},{0,1},{0,0}},{{0,1},{1,1},{1,0},{0,0}},{{1,0},{0,0},{1,0},{0,0}}};

int chainorder[numcountbal][numchain]={{3,0,1,3},{3,1,3,0},{3,3,0,1}};
int nwp[stimperchaintotal]={25,75,125,175,225,275,325,375};
int nbp[stimperchaintotal]={375,325,275,225,175,125,75,25};
int length[stimperchaintotal]={14,21,30,44,64,92,134,193};
int bwg[3]={0,255,160};
long seed,orig_seed;

int stim,cycle,totalcycle,totaltrial;
int mset,session;
main()
{
  int t,c,countbal,ch,flag;
  int order[stimperchain];
  int chain;
  char *fontlist;      

  image *newchain_i; 
  movie *newchain_m;

  mylog=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,mylog);
  seed=-(2233+(mylog->subjnum%100)/(numcountbal*numsets));
  orig_seed=seed;
  mset=(mylog->subjnum%100)%numsets;
  /*mset=0;*/
  countbal=((mylog->subjnum%100)/numsets)%numcountbal;
  
  printf("Which session (0=1rst day, 1=2nd day)\n");
  scanf("%d",&session);
  fontlist=getenv("FONTLIST");
  for (t=0;t<4;t++)
    printf("here %d %d %d %d\n",mset,t,session,group[mset][t][session]);
  printf("seed=%ld countbal=%d set=%d session=%d\n",seed,countbal,mset,session);
  getchar();

  SetupMoviePackage(fontlist);

  makePalette(GRAYSCALE);


/*  geo_pixels=(char **)malloc(sizeof(char *)*stimperchaintotal); 
  for (t=0;t<stimperchain;t++)
   {
    geo_pixels[t]=(char *)malloc(sizeof(char)*(83*83));
    if (geo_pixels[t]==NULL) {CleanupMoviePackage();printf("geo badpointer");exit(1);}
   }
  readP1PPM(geo_pixels[0],"..\\pbmpics\\an.pbm");
  readP1PPM(geo_pixels[1],"..\\pbmpics\\ban.pbm");
  readP1PPM(geo_pixels[2],"..\\pbmpics\\gan.pbm");
  readP1PPM(geo_pixels[3],"..\\pbmpics\\don.pbm");
  readP1PPM(geo_pixels[4],"..\\pbmpics\\khani.pbm");
  readP1PPM(geo_pixels[5],"..\\pbmpics\\tan.pbm");
  readP1PPM(geo_pixels[6],"..\\pbmpics\\kar.pbm");
  readP1PPM(geo_pixels[7],"..\\pbmpics\\vin.pbm");
 */
 
  /*CleanupMoviePackage(); exit(1);*/


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

  for (stim=0;stim<stimperchain;stim++)
    order[stim]=stim;

/*  runMovie(newchain_m,UNTIL_RESPONSE,1);
  CleanupMoviePackage(); exit(1);*/

  for (ch=0;ch<numchain;ch++)
    {
      seed=orig_seed+ch;
      totalcycle=0;
      totaltrial=0;
      chain=chainorder[countbal][ch];
      runMovie(newchain_m,UNTIL_RESPONSE,1);
      /*CleanupMoviePackage();
      exit(1);*/
      flag=1;
      do
	{
	  if (flag==1) cycle=0;
	  else cycle++;
	  flag=0;
	  distribute(order,stimperchain,&seed);
	  for (stim=0;stim<stimperchain;stim++)
	    if (present(chain,order[stim])==0) flag=1;
	  totalcycle++;
	} while (flag==1 || cycle<1);
    } 
  thankyou();
  CleanupMoviePackage();
}

int present(int chain,int stim)
{
  totaltrial++;
  if (present_stim(chain,stim,0)==1)
    return(1);
  else
    {
      present_stim(chain,stim,1);
      return(0);
    }
}
	
      
      


int present_stim(int j, int a, int feedback)
{
  char junk[85],respstring[3],outline[80];
  int resp,correct,realstimno,alpha;
  response *data;
  FILE *outfile;

  alpha=group[mset][j][session];
  realstimno=oldnew[alpha][a];
  if (realstimno<0 || realstimno>7) {CleanupMoviePackage();exit(1);}
  downloadImage(targ_i);
  clearPicBuf();
  switch (j)
    {
    case 0: hline(xcenter-length[realstimno]/2,xcenter-length[realstimno]/2+length[realstimno],ycenter,picBuf); break;
    case 1: fillPicBuf(bwg[2]),putrandombox(nbp[realstimno],nwp[realstimno],20,xcenter,ycenter,bwg,picBuf,&seed);break;
    case 2: putgeolet(geo_pixels[realstimno],picBuf);break;
    case 3: putshape(a,picBuf);break;
    }

  if (feedback) 
    {
      sprintf(junk,"%d",a+1);
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
      data=runMovie(stim_m,FULL_SHOW,1);
      switch (data->x[0].resp){
      case '1': resp=0;break;
      case '2': resp=1;break;
      case '3': resp=2;break;
      case '4': resp=3;break;
      case '5': resp=4;break;
      case '@': resp=5;break;
      default : resp=9;break;}
      if (resp==a) 
	{
	  audio(CORRECT);
	  correct=1; 
	}
      else correct=0;
      sprintf(outline,"%04d %d %d %d %d %d %d %d %d %0d %02d\n",
	      mylog->subjnum,session,j,realstimno,a,resp,correct,cycle,stim,totalcycle,totaltrial);
      outfile=fopen(mylog->outfn,"at");
      fprintf(outfile,"%s",outline);
      fclose(outfile);
      if (resp==5) {CleanupMoviePackage();exit(1);}
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


void putshape(int a,unsigned char far *b)
{
  switch (a)
    {
    case 0: putsquare3(15,xcenter,ycenter,255,b);break;
    case 2: putcircle5(10,xcenter,ycenter,255,b);break;
    case 4: puttriangle3(10,xcenter,ycenter,255,b);break;
    case 3: putcross(10,xcenter,ycenter,255,b);break;
    case 1: drawText("@",xcenter,ycenter,0,255); break;
    }
}
