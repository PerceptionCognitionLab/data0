#include <stdio.h>        
#include <stdlib.h>
#include <string.h> /*for memcpy command*/
#include "movieobj.h"
#include "video.h"
#include <mem.h>
#include "exptlib.h"
#include <math.h>
#include <time.h>

#define PI 3.14159265892
#define e 2.718281828
#define Lm0 127
#define fc0 1.5
#define sd0 1
#define gaborside 200

void makegabor(float *x, float Lm, float Cp, float fc, float sd, unsigned char far *b);
float gabor(float x, float y, float Lm, float Cp, float fc, float sd);
unsigned char gabor2(float x, float y, float Lm, float Cp, float fc, float sd);
void picbufout(unsigned char far *b, FILE *out);
int gabor4(double x, double y, double Lm, double Cp, double fc, double sd, double radius);
int sign(float x);
unsigned char redgreen(float propred);

main(){
unsigned char gab;
float Cp=.05;
float x0=0;
float y0=0;
FILE *dump;
float x[gaborside];
char *fontlist;
int i,j;
float radius0=1;
float f=.2;

//dump=fopen("dumptest.pbm","wt"); 
  
for (i=0;i<gaborside;i++){
   x[i]=-5+(.05*i);
}

//fontlist=getenv("FONTLIST");
//SetupMoviePackage(fontlist);

for(i=0;i<gaborside;i++){
for(j=0;j<gaborside;j++){
gab=redgreen(f);
printf("%i %i - %i\n",j,i,(int)(gab));
}
}

//makegabor(x, Lm0, Cp, fc0, sd0, picBuf);
//picbufout(picBuf,dump);
//CleanupMoviePackage();

}

void makegabor(float *x, float Lm, float Cp, float fc, float sd, unsigned char far *b)
{
        int i,j,x0,y0;
        x0=xcenter-(gaborside/2)+1;
        y0=ycenter-(gaborside/2)+1;
        for (i=0;i<gaborside;i++){
                for (j=0;j<gaborside;j++){
                        b[o(j+x0,i+y0)-1]=gabor2(x[j],x[i],Lm,Cp,fc,sd);
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
  gab=Lm*(1+(Cp*cos(2*PI*x*fc)*exp(-.5*((x/sd)*(x/sd))-.5*((y/sd)*(y/sd)))));
  return (gab);
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
}

int gabor4(double x, double y, double Lm, double Cp, double fc, double sd, double radius)
{
  double gpart=0,x1=0,y1=0,gab;
  int signx=sign(x);
  int signy=sign(y);
  if((sqrt(pow(x,2)+pow(y,2)))<radius){
    gpart=1;
  }else{
    if(x!=0){
        x1=signx*radius*cos(atan2(abs(y),abs(x)));
        y1=signy*radius*sin(atan2(abs(y),abs(x)));
    }else{
        x1=0;
        y1=radius;
    }
    gpart=exp(-.5*pow((x-x1)/sd,2)-.5*pow((y-y1)/sd,2));
  }
  gab=Lm*(1+(Cp*cos(2*PI*x*fc)*gpart));
  return((int)(gab));
}

int sign(float x){
int sign=0;
if (x<0){sign=-1;}
if (x==0){sign=0;}
if (x>0){sign=1;}
return sign;
}

unsigned char redgreen(float propred){
float f;
int ret;
f=(1.*rand()/RAND_MAX);
printf("F=%f\n",f);
if (f>propred){
        ret=254;
}else{
        ret=1;
}
return((unsigned char)(ret));
}

