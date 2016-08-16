// Ric Rodriguez rirrodri
// $Id: linesort.c,v 1.3 2016-03-09 22:49:03-08 - - $
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "inssort.h"
void printarray(char** array, int nelem){
 for(int itor=0;itor<nelem;itor++){
  printf("%s\n", array[itor]);
  free(array[itor]);
 }
}
int strCmp(const void* this, const void *that){
 char** thisstr=(char**) this;
 char** thatstr=(char**) that;
 int result= strcmp(*thisstr, *thatstr);
 return result;
} 
int main(int argc, char** argv){
 int exit_status=EXIT_SUCCESS;
 char *array[0x1000];
 int arrayLen=0;
 for(int itor=0; itor<4096;itor++){
  char buffer[0x1000];
  char *gotline = fgets (buffer, sizeof buffer, stdin);
  if (gotline == NULL) break;
  char *nlpos = strchr (buffer, '\n');
  if (nlpos != NULL) {
  *nlpos = '\0';
  }
  array[itor]=strdup(buffer);
  assert(array[itor]!=NULL);
  arrayLen++;
 }
 inssort(array, (size_t) arrayLen, sizeof *array, strCmp);
 printarray(array, arrayLen);
 return exit_status;
}
