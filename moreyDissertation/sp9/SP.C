#include "sp.h"

extern long double rtSlope;
extern movie *std_m;
extern image *std_i;

void die(char *message){
cleanup();
printf("%s",message);
exit(1);
}

void distributeChar(char q[], int size, int *seed)
{

  int c,newplace;
  char temp;

  for (c=0;c<size;c++)
    {
      newplace=randint(c,size-1,seed);
      temp=q[newplace];
      q[newplace]=q[c];
      q[c]=temp;
    }
}


void loadwordlist(int cat,FILE *infile)
{
  int i;

  for (i=0;i<NWORDS;i++)
    {
      fscanf(infile,"%s",words[cat][i]);
      //printf("%s\n",words[cat][i]);
    }
}



void runMovieTextBG(char *message,int x, int y, int font, int color,int colorBG)
{
  fillImage(std_i,colorBG);
  drawText(std_i,message,x,y,font,color);
  setMovie(std_m,0,std_i,1);
  runMovie(std_m,UNTIL_RESPONSE,1);
}

void runMovieTimedTextBG(char *message,int x, int y, int font, int color, int colorBG,int frames)
{
  fillImage(std_i,colorBG);
  drawText(std_i,message,x,y,font,color);
  setMovie(std_m,0,std_i,frames);
  runMovie(std_m,FULL_SHOW,0);
}



int colVal(int red,int green,int blue){
	return((red*256*256)+(256*green)+(blue));
}


int doTrial(trialParams *oneTrial, int *seed){
  
  int i,j,resp,tooFast=0,targADJ=(oneTrial->stimFrameDurs[PREBLANK]==0);
  float RT;
  char temp[MAXCHAR+1];

  //Clear image
  for(i=0;i<NFRAMES;i++)
  clear_to_color(*(oneTrial->stim_i[i]),0);

  //FIXATION
  
  vline(*(oneTrial->stim_i[FIXATION]),oneTrial->xcen+oneTrial->xFixAdj,oneTrial->ycen+oneTrial->yFixAdj-oneTrial->fixationSize,oneTrial->ycen+oneTrial->yFixAdj+oneTrial->fixationSize,255);
  hline(*(oneTrial->stim_i[FIXATION]),oneTrial->xcen+oneTrial->xFixAdj-oneTrial->fixationSize,oneTrial->ycen+oneTrial->yFixAdj,oneTrial->xcen+oneTrial->xFixAdj+oneTrial->fixationSize,255);

  //PREMASK
  drawText(oneTrial->stim_i[PREMASK],oneTrial->preMask,oneTrial->xcen,oneTrial->ycen,oneTrial->fontNum,255);

  //PRIME
  drawText(oneTrial->stim_i[PRIME],oneTrial->primeWord,oneTrial->xcen,oneTrial->ycen,oneTrial->fontNum,255);
 
  //POSTMASK
  drawText(oneTrial->stim_i[POSTMASK],oneTrial->postMask,oneTrial->xcen,oneTrial->ycen,oneTrial->fontNum,255);

  //TARGET
  drawText(oneTrial->stim_i[TARGET],oneTrial->targWord,oneTrial->xcen,oneTrial->ycen,oneTrial->fontNum,255);
  //drawText(oneTrial->stim_i[TARGET],"Test",oneTrial->xcen,oneTrial->ycen,1,0);
  
  for(i=0;i<NFRAMES;i++){
    if(i==PREBLANK || i==TARGET){
      setMovie(oneTrial->trial_m,i,oneTrial->stim_i[i+targADJ],oneTrial->stimFrameDurs[i+targADJ]);
    }else{
      setMovie(oneTrial->trial_m,i,oneTrial->stim_i[i],oneTrial->stimFrameDurs[i]);
    }
  }
  oneTrial->data=runMovie(oneTrial->trial_m,UNTIL_RESPONSE,1);
  
  if ((oneTrial->trial_m->x[TARGET-targADJ].ts)==0) RT=-1;
  else RT=(oneTrial->data->x[0].rt-oneTrial->trial_m->x[TARGET-targADJ].ts)*rtSlope;

  if (RT<oneTrial->minRT)
    {
      clearScreen(0);
      sound(75);
      delay(4000);
      nosound();
      runMovieText("Too fast! Press Enter.",oneTrial->xcen, oneTrial->ycen, oneTrial->fontNum,255);
      tooFast=1;
    }

  
  switch(oneTrial->data->x[0].resp & 0x00ff){
  case 'z': resp=0; break;
  case 'Z': resp=0; break;
  case '/': resp=1; break;
  case '?': resp=1; break;

  case '@': 
    cleanup();
    printf ("Stopped while running by participant\n");
    exit(1);
    break;
  default: 
    resp=-1;
    sound(75);
    delay(1000);
    nosound();
    runMovieText("Invalid response! Press Enter.",oneTrial->xcen, oneTrial->ycen, oneTrial->fontNum, 255);
    break;
  }  

  if(oneTrial->data->x[0].resp>=pow(2,15))
  {
    resp=-2;
    sound(75);
    delay(1000);
    nosound();
    runMovieText("Multiple responses! Press Enter.",oneTrial->xcen, oneTrial->ycen, oneTrial->fontNum,255);
  }
  
  //if(tooFast) resp=-1;
  
  if(!tooFast && oneTrial->feedback){
    if((oneTrial->tid && (oneTrial->targCat==resp)) || (!oneTrial->tid && (oneTrial->primeCat==resp))){
      sound(1200);
      delay(75);
      nosound();
      sound(1800);
      delay(75);
      nosound();
    }else{
      sound(90);
      delay(oneTrial->badFBLength);
      nosound();
    }
  }
    
  if(oneTrial->tid){
    if(oneTrial->targCat==resp){
      oneTrial->acc=1;
    }else{
      oneTrial->acc=0;
    }
  }else{
    if(oneTrial->primeCat==resp){
      oneTrial->acc=1;
    }else{
      oneTrial->acc=0;
    }
  }


  oneTrial->response=resp;
  oneTrial->RT=RT;
  
  return 0;
}
