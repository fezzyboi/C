// Ric Rodriguez rirrodri
// $Id: numsort.c,v 1.3 2016-03-09 22:49:03-08 - - $
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "inssort.h"

void printarray(double *array, int nelem){
 for(int itor=0;itor<nelem;itor++){
  printf("%20.15g\n", array[itor]);
 }
}

int cmpTo(const void* this, const void* that){
 return (*(double*)this-*(double*)that);
}
int main(int argc, char** argv){
 int exit_status=EXIT_SUCCESS;
 double array[0x1000];
  int arrayLen=0;
 for(int itor=0;itor<4096;itor++){
  double tmp;
  int goodInput=scanf("%lf", &tmp);
  if(goodInput!=1){break;}
  array[itor]=tmp;
  arrayLen++;
 }
 const size_t nelem=sizeof array /sizeof *array;
 inssort(array,(size_t)arrayLen, sizeof *array, cmpTo);
 printarray(array, arrayLen);
 return exit_status;
}

