// Ric Rodriguez rirrodr
// $Id: inssort.h,v 1.1 2016-03-09 06:26:02-08 - - $
#ifndef __INSSORT_H_
#define __INSSORT_H_
#include <stdio.h>
#include <stdlib.h>

void inssort(void* base, size_t nelem, size_t size, int (*compar)
 (const void*, const void*));
#endif
