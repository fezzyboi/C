// Ric Rodriguez rirrodri
// $Id: bigint.c,v 1.5 2016-03-01 05:52:32-08 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}
bool arrayCompare(bigint *this, bigint*that) {
   if(this->size > that->size){
      return true;
   }
   else if(that->size > this->size){
      return false;
   }
   else if(this->size == that->size){
      for(int i = this->size; i >= 0; i++){
         if(this->digits[i] > that->digits[i]){
            return true;
         }
         if(this->digits[i] < that->digits[i]){
            return false;
         }
      }
   }
   return true;
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *new_string_bigint (const char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   const char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *do_sub (bigint *this, bigint *that) {
   bigint *sum= new_bigint(this->capacity);
   sum->size=this->size;
   size_t len=sum->capacity;
   int car=0;
   int dig=0;
   for(size_t i=0;i<len;i++){
      dig=10+this->digits[i]-car;
      if(i<that->capacity){
         dig=dig-that->digits[i];
      }
      sum->digits[i]=dig%10;
      car=1-dig/10;
   }
   trim_zeros(sum);
   return sum;
}

bigint *do_add (bigint *this, bigint *that) {
   size_t larger=0;
   if(this->size>that->size){
      larger=this->capacity;
   }else{
      larger=that->capacity;
   }
   bigint *sum= new_bigint(larger+1);
   sum->size=sum->capacity;
   int car=0;
   int dig=0;
   for(size_t i=0;i<larger;++i){
      dig=car;
      if(i<this->capacity)
         dig+=this->digits[i];
      if(i<that->capacity)
         dig+=that->digits[i];
      sum->digits[i]=dig%10;
      car=dig/10;
   }
   trim_zeros(sum);
   return sum;
}
void free_bigint (bigint *this) {
   
}

void print_bigint (bigint *this) {
   if(this->negative==true){
      printf("%s", "-");
   }
   int count;
   for(char *byte=&this->digits[this->size-1];
      byte>=this->digits;--byte){
      printf ("%d", *byte);
      ++count;
      if(count>70){
         printf("%s\n", "\\");
         count=0;
      } 
   }
   printf("\n");
}

bigint *add_bigint (bigint *this, bigint *that) {
   bigint* result=NULL;
   if(this->negative==that->negative){
      result=do_add(this, that);
      result->negative=this->negative;
   }else{
      result=do_sub(arrayCompare(this,that)?this:that,
      arrayCompare(this,that)?that:this);
      result->negative = arrayCompare(this, that) ?
      this->negative:that->negative;
   }      
   return result;
}

bigint *sub_bigint (bigint *this, bigint *that) {
   bigint *result=NULL;
   if(this->negative!=that->negative){
      result=do_add(this, that);
      result->negative=this->negative;
   }else{
      result=do_sub(arrayCompare(this, that)?this:that,
      arrayCompare(this, that)?that:this);
      result->negative = arrayCompare(that, this) ?
      !this->negative:that->negative;
   }      
   return result;
}

bigint *mul_bigint (bigint *this, bigint *that) {
   size_t intSize=this->size+that->size;
   bigint *result=new_bigint(intSize);
   result->size=intSize;
   result->negative=this->negative!=
   that->negative? true:false;
   for(size_t i=0;i<this->size;i++){
      int remainder=0;
      for(size_t j=0;j<that->size;j++){
         int ptr=result->digits[i+j];
         ptr+=this->digits[i]*that->digits[j];
         ptr+=remainder;
         result->digits[i+j]=ptr%10;
         remainder=ptr/10;
      }
      result->digits[i+that->size]+=remainder;
   }
   trim_zeros(result);
   return result;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

