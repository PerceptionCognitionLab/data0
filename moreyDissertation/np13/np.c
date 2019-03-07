#include "np.h"

extern long double rtSlope;
extern movie *std_m;
extern image *std_i;


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
  
  int cor,i,j,resp,tooFast=0;
  float RT;
  char temp[15];


  //Clear image
  for(i=0;i<NFRAMES;i++)
  clear_to_color(*(oneTrial->stim_i[i]),0);

  //FIXATION
  
  vline(*(oneTrial->stim_i[FIXATION]),oneTrial->xcen,oneTrial->ycen-oneTrial->fixationSize+oneTrial->fixYOffset,oneTrial->ycen+oneTrial->fixationSize+oneTrial->fixYOffset,255);
  hline(*(oneTrial->stim_i[FIXATION]),oneTrial->xcen-oneTrial->fixationSize,oneTrial->ycen+oneTrial->fixYOffset,oneTrial->xcen+oneTrial->fixationSize,255);

  //PREMASK
  
  drawText(oneTrial->stim_i[PREMASK],"#",oneTrial->xcen,oneTrial->ycen,1,255);
  

  //PRIME
  
  sprintf(temp,"%i",oneTrial->primeNum);
  drawText(oneTrial->stim_i[PRIME],temp,oneTrial->xcen,oneTrial->ycen,1,255);


  //PostMASK
  drawText(oneTrial->stim_i[POSTMASK],"#",oneTrial->xcen,oneTrial->ycen,1,255);


  //TARGET
  sprintf(temp,"%i",oneTrial->targetNum);
  drawText(oneTrial->stim_i[TARGET],temp,oneTrial->xcen,oneTrial->ycen,1,255);

   
  for(i=0;i<NFRAMES;i++)
  setMovie(oneTrial->trial_m,i,oneTrial->stim_i[i],oneTrial->stimFrameDurs[i]);
  
  oneTrial->data=runMovie(oneTrial->trial_m,UNTIL_RESPONSE,1);
  
  if ((oneTrial->trial_m->x[TARGET].ts)==0) RT=-1;
  else RT=(oneTrial->data->x[0].rt-oneTrial->trial_m->x[TARGET].ts)*rtSlope;

  if (RT<oneTrial->minRT)
    {
      clearScreen(0);
      sound(75);
      delay(4000);
      nosound();
      runMovieText("Too fast! Press Enter.",oneTrial->xcen, oneTrial->ycen, oneTrial->fontNum, 255);
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
    runMovieText("Multiple responses! Press Enter.",oneTrial->xcen, oneTrial->ycen, oneTrial->fontNum, 255);
  }
  
  //if(tooFast) resp=-1;

  if(oneTrial->targetID){
    oneTrial->cor=oneTrial->targetNum/5;
  }else{
    oneTrial->cor=oneTrial->primeNum/5;
  }

  if(!tooFast && oneTrial->feedback){
	if(resp != oneTrial->cor){
		sound(90);
		delay(oneTrial->wrongSoundLength);
		nosound();
	}else{
		sound(1200);
		delay(75);
		nosound();
		sound(1800);
		delay(75);
		nosound();
	}
  }

 
  oneTrial->response=resp;
  oneTrial->RT=RT;

  return 0;
}
