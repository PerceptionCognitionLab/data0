#include <allegro.h>
#include <alfont.h>
#include "elib.h"
#include "exptlib.h"
#include <stdlib.h>
#include <stdio.h>

#define NFRAMES 7


#define FIXBLANK 0
#define FIXATION 1
#define PREMASK 2
#define PRIME 3
#define POSTMASK 4
//#define PREBLANK 5
#define TARGET 5
#define POSTBLANK 6

int colVal(int red,int green,int blue);

typedef struct
{
  int stimFrameDurs[NFRAMES];
  movie *trial_m;
  image *stim_i[NFRAMES];  
  float RT,minRT;
  int xcen,ycen,fixationSize;
  response *data;
  int fontNum, feedback ,response,targetNum,primeNum;
  int fixYOffset,targetID,wrongSoundLength,cor;
} trialParams;




int doTrial(trialParams *oneTrial,int *seed);

