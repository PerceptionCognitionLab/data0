#include "mylib.h"

#define numchain 3
#define numtrials 501
#define bottomnumber 11
#define topnumber 80
#define totalnumber (topnumber-bottomnumber)

int chainlength[numchain]={26,27,15};

main()
{
  int to[totalnumber],t,c,j,i;
  int na[numchain][30],co[numchain][30],current[numchain];
  int flag[numchain],top[numchain],success;

  long seed=-123;

  /* assign orders*/
  for (t=bottomnumber;t<topnumber;t++)
    to[t-bottomnumber]=t;
  distribute(to,totalnumber,&seed);
  c=0;
  for (j=0;j<numchain;j++)
    {
      for (i=0;i<chainlength[j];i++)
	{
	  na[j][i]=to[c];
	  co[j][i]=i;
	  c++;
	}
      distribute(co[j],chainlength[j],&seed);
    }
  

  /*display first three*/

  for (j=0;j<numchain;j++)
    {
      present(j,co[j][0],na[j][0],1);
      flag[j]=0;
      current[j]=0;
      top[j]=0;
    }

  for (t=0;t<numtrials;t++)
    {
      j=t%numchain;      
      success=present(j,co[j][current[j]],na[j][current[j]],0);
      printf("output trial=%d chain=%d stim=%d resp=%d ans=%d\n",t,j,
	     co[j][current[j]],na[j][current[j]],success);
      if (success==0) flag[j]=1;
      current[j]++;
      printf("internal current=%d top=%d flag=%d\n",current[j],top[j],flag[j]);
      if (current[j]>top[j])
	{
	  if (flag[j]==0)
	    {
	      top[j]++;
	      present(j,co[j][top[j]],na[j][top[j]],1);
	      
	    }
	  distribute(co[j],top[j]+1,&seed);
	  current[j]=0;
	  flag[j]=0;
	}
    }
}
  

int present(int j, int a, int b, int feedback)
{
  char x1,x2;
  char junk[85];
  int resp;

  printf("chain %d stimulus %d ",j,a);
  if (feedback) printf("response %d\n",b);
  else
    {
      gets(junk);
      resp=atoi(junk);
      printf("%d\n",resp);
      if (resp==b) return 1;
      else return 0;
    }
}













