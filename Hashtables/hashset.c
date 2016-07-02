// Ric Rodriguez rirrodri
// $Id: hashset.c,v 1.9 2014-05-15 20:01:08-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %zd, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   for(size_t itor=0;itor<this->size;itor++){
      hashnode *toFree=this->chains[itor];
      while(toFree!=NULL){
         hashnode *new=toFree;
         free(toFree->word);
         toFree=toFree->link;
         free(new);

      }
   }
   free(this->chains);
   free(this);
}

void put_hashset (hashset *this, const char *item) {
   if(this->load*2>this->size){
      size_t thisSize=this->size;
      this->size=(this->size*2)+1;
      size_t chainSize=this->size*sizeof(hashnode*);
      hashnode **newChain=malloc(chainSize);
      memset(newChain,0,chainSize);
      for(size_t itor=0;itor<thisSize;itor++){
         hashnode *chainLink=this->chains[itor];
         while(chainLink!=NULL){
            int hash =strhash(chainLink->word)%this->size;
            hashnode *temp=malloc(sizeof(struct hashnode));
            temp->word=strdup(chainLink->word);
            temp->link=(newChain[hash]==NULL?NULL:newChain[hash]);
            newChain[hash]=temp;
            free(chainLink->word);
            hashnode *new=chainLink;
            chainLink=chainLink->link;
            free(new);
         }
      }
      free(this->chains);
      this->chains=newChain;
   }
   int hash=strhash(item)%this->size;
   hashnode *first=this->chains[hash];
   if(has_hashset(this,item))return;
   hashnode *latter=malloc(sizeof(struct hashnode));
   latter->word=strdup(item);
   latter->link=(first==NULL?NULL:this->chains[hash]);
   this->chains[hash]=latter;
   this->load++;
}

bool has_hashset (hashset *this, const char *item) {
   int hash=strhash(item)%this->size;
   hashnode *compare=NULL;
   for(compare=this->chains[hash];compare!=NULL;compare=compare->link){
      int result=strcmp(compare->word, item);
      if (result==0)return true;
   }return false;

}

