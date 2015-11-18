#include <allegro.h>
#include <alfont.h>
#include "elib.h"
#include "exptlib.h"
#include <stdlib.h>
#include <stdio.h>


#define MAXN 8

int colVal(int red,int green,int blue);

typedef struct
{
  movie *trial_m,*feedback_m;
  image *blank_i, *first_i, *probe_i,*feedback_i,*fixate_i,*mask_i;
  int predur,firstdur,interdur,probedur,maskdur,postdur,N,twoPaneldur,twoPanelBoxColor,fixatedur;
  int xlocations[MAXN],ylocations[MAXN],maxColor,newColor,feedback,doProbe,response,fixateColor,feedbackLineColor,doFeedbackLine;
  int colors[MAXN],twoPanelFeedback,lPanelxcen,rPanelxcen,lPanelycen,rPanelycen,twoPanelBox;
  int changedSquare,isChange,xcen,ycen,fontNum,squareSize,xSize,ySize,probeColor,minDist,probeSquare;
  int drawNonCued,trialType;
  float probeScale,boxScale,RT,minRT,fixateScale;
  response *data;
  int maskDivNum,*maskCols;
} luckParams;

typedef struct
{
  movie *pie_m;
  image *pie_i;
  int x,y,lineColor,color1,color2,piedur,textColor,fontNum;
  float scale,proportion;
} pieParams;



int doLuckTrial(luckParams *luckTrial,int *seed);
int drawPie(pieParams *myPie);
