/* This file contains the source code for ECE 368 Project #1 on priority queues

  Written by: Michael Baio and Igal Fleggman
  
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



int main()
{
  int i = 0;
  double arr[10001];
  
  while(i < 10000)
  {
    arr[i] = -1 * log(1-((double)rand() / (double)RAND_MAX)); 
    printf("%lf\n",arr[i]);
    i++;
  }
  
  i = 0;
  float max = 0;
  while (i < 10000)
  {
    if (arr[i] > max)
      max = arr[i];
    i++;
  }
  printf("Max: %lf\n",max);
  return 0;
}
