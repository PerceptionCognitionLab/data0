#include "mylib.h"

#define numchain 4
#define bottomnumber 11
#define topnumber 80
#define totalnumber (topnumber-bottomnumber)
#define stimperchain 4

int present(int chain,int stim,int resp);
int present_stim(int j, int a, int b, int feedback);


main()
{
  int t,c,resp_temp[100];
  int resp[numchain][stimperchain],order[stimperchain];
  int chain,stim,cycle;


  long seed=-123;

  /* assign response terms*/
  for (t=bottomnumber;t<topnumber;t++)
    resp_temp[t-bottomnumber]=t;
  distribute(resp_temp,totalnumber,&seed);



  c=0;
  for (chain=0;chain<numchain;chain++)
    for (stim=0;stim<stimperchain;stim++)
      {
	resp[chain][stim]=resp_temp[c];
	c++;
      }



  for (stim=0;stim<stimperchain;stim++)
    order[stim]=stim;

  chain=0;
  do
    {
      cycle=0;
      stim=0;
      distribute(order,stimperchain,&seed);
      while (present(chain,order[stim],resp[chain][order[stim]])==1 && cycle<2)
	{
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
  char junk[85];
  int resp;
  
  printf("chain %d stimulus %d ",j,a);
  if (feedback) 
    {
      printf("response %d\n",b);
      return(0);
    }
  else
    {
      gets(junk);
      resp=atoi(junk);
      printf("%d\n",resp);
      if (resp==b) return 1;
      else return 0;
    }
}

  
