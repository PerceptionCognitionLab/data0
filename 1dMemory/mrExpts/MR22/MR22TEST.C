#include <stdio.h>       
#include <stdlib.h>
#include <math.h>

#define status STAND_ALONE
#define feedbacklength 42
#define WRONG 0
#define RIGHT 1
#define numtrialsperblock 10
#define stimperblock 12
#define numblocks 10
#define numlength 16
#define maxmovex 6
#define maxmovey 6
#define breaklength 5
#define fore_time 30
#define totaltrials numtrialsperblock*numblocks

int main(){
 int stimset,i=0,j=0,k=0,movepixelx=0,movepixely=0,l,cor,trlnum,sub=1,halftrials=totaltrials/2;
 for (i=0;i<numblocks;i++){
 for (k=0;k<numtrialsperblock;k++){
	trlnum=(i*numtrialsperblock)+k;
	stimset=( (sub+1+(trlnum/halftrials)) /2)%2;
	printf("set: %i\n",stimset);
 } 
 }
}
