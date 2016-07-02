// Ric Rodriguez rirrodri
// $Id: spellchk.c,v 1.9 2014-05-15 21:07:47-07 - - $

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "debug.h"
#include "hashset.h"
#include "yyextern.h"


#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2
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
void print_error (const char *object, const char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", program_name, object, message);
   fflush (NULL);
   exit_status = EXIT_FAILURE;
}

FILE *open_infile (const char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL){
    print_error (filename, strerror (errno));
    exit_status=2;}
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (const char *filename, hashset *hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      if(!has_hashset(hashset,yytext)){
        char* stringToLower=strdup(yytext);
        for(int itor=0;stringToLower[itor]!='\0';itor++)
          stringToLower[itor]=tolower(stringToLower[itor]);
          if(!has_hashset(hashset, stringToLower)){
            printf("Spelling error: %s\n", yytext);
            exit_status=1;

          
        }          free(stringToLower);

      }
   }
}

void load_dictionary (const char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;

  DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
  FILE *dictIn= open_infile(dictionary_name);
  if(dictIn!=NULL){
  for(;;){
    char buffer[256];
   char *gotline = fgets (buffer, sizeof buffer, dictIn);
   if (gotline == NULL) break;
   char *nlpos = strchr (buffer, '\n');
   if (nlpos != NULL) {
   *nlpos = '\0';
   }
   put_hashset(hashset, buffer);
}
fclose(dictIn);
}
}
int x=0;
void scan_options (int argc, char** argv,
                   char **default_dictionary,
                   char **user_dictionary) {
   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': *user_dictionary = optarg;
                   break;
         case 'n': *default_dictionary = NULL;
                   break;
         case 'x': x++;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debug_flags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }
}

int main (int argc, char **argv) {
   program_name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;
   scan_options (argc, argv, &default_dictionary, &user_dictionary);

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   if(x==1){
    size_t chain[hashset->size];
    memset(chain,0,hashset->size*sizeof(size_t));
    printf("%5zd words in hash set\n", hashset->load);
    printf("%5zd size of the hash array\n", hashset->size);
    for(size_t itor=0;itor<hashset->size;itor++){
      hashnode *tmp=hashset->chains[itor];
      size_t size =0;
      while(tmp!=NULL){
        size++;
        tmp=tmp->link;
      }
      if(size>0){
        chain[size]++;
      }
    }
    for(size_t itor=0;itor<hashset->size;itor++){
      if(chain[itor]!=0){
        printf("%5zd chains of size %5zd\n",chain[itor], itor);
      }
    }


   }else if(x==2){
    for(size_t itor=0;itor<hashset->size;itor++){
      hashnode *tmp = hashset->chains[itor];
      int i = 0;
      while(tmp != NULL) {
         if(i == 0){
            printf("array[%10zd] = %20lu \"%s\"\n",itor, 
strhash(tmp->word), tmp->word);
          }
         else{
            printf("                  = %20lu \"%s\"\n",
               strhash(tmp->word), tmp->word);
          }
         tmp = tmp->link;
         i++;
      }
   }


   }
   free_hashset(hashset);
   yylex_destroy ();
   return exit_status;
}

