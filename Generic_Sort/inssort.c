// Ric Rodriguez rirrodri
// $Id: inssort.c,v 1.2 2016-03-09 22:49:03-08 - - $
#include <stdlib.h>
#include "inssort.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void inssort(void* base, size_t nelem, size_t size,
              int (*compar) (const void*, const void*)) {
   void* copy=malloc(size);
   for (size_t index = 1; index < nelem; ++index) {
      size_t slot=index;
 memcpy(copy, (char*)base+(slot*size), size);
 for(; slot>0;--slot){
  int cmp=compar(copy,(char*)base+(slot-1)*size);
  if(cmp>0){break;}
  memcpy(base+(slot*size), base+(slot-1)*size, size);
 }
 memcpy(base+(slot*size), copy, size);
  
}
free(copy);
}

