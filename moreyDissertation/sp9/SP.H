#include <allegro.h>
#include <alfont.h>
#include "elib.h"
#include "exptlib.h"
#include <stdlib.h>
#include <stdio.h>

#define NFRAMES 8
#define NWORDS 30
#define MAXCHAR 25

#define FIXBLANK 0
#define FIXATION 1
#define PREMASK 2
#define PRIME 3
#define POSTMASK 4
#define PREBLANK 5
#define TARGET 6
#define POSTBLANK 7

char words[2][NWORDS][MAXCHAR+1];

void die(char *message);
int colVal(int red,int green,int blue);

typedef struct
{
  int stimFrameDurs[NFRAMES];
  movie *trial_m;
  image *stim_i[NFRAMES];  
  float RT,minRT;
  int xcen,ycen,fixationSize,xFixAdj,yFixAdj,feedback,badFBLength;
  response *data;
  int fontNum, response,targNum,targCat,primeNum,primeCat,acc,pdur,tid;
  char targWord[MAXCHAR],primeWord[MAXCHAR],preMask[MAXCHAR],postMask[MAXCHAR];
} trialParams;




int doTrial(trialParams *oneTrial,int *seed);
void myTriangle(BITMAP *bitmap, int x1, int y1, int x2, int y2, int x3, int y3, int color);
void loadwordlist(int cat, FILE *infile);
void distributeChar(char q[], int size, int *seed);


