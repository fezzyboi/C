// Ric Rodriguez
// CMPS101-PA2
// rirrodri@ucsc.edu
// Lex.c

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include <string.h>

#define MAX_LEN 1024

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Usage: Lex [infile] [outfile]\n");
		exit(1);
	}
	FILE *in, *out;
	char line[MAX_LEN];
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	if(in == NULL || out == NULL){
		printf("Unable to open file\n");
		exit(1);
	}
	int lineCount=0;
	while(fgets(line, MAX_LEN, in)!=NULL){
		lineCount++;
	}
	rewind(in);
	char inputLines[lineCount-1][MAX_LEN];
	int lineIndex=0;
	while(fgets(line, MAX_LEN, in)!=NULL){
		strcpy(inputLines[lineIndex++],line);
	}
	List l = newList();
	append(l, 0);
	for(int j = 1; j<lineCount ; j++){
		char *key = inputLines[j];
		int i = j-1;
		moveBack(l);
		while( i>=0 && strcmp(key,  inputLines[get(l)])<=0) {
			i--;
			movePrev(l);
		}
		if(index(l)>=0){
			insertAfter(l, j);
		} else {
			prepend(l, j);
		}
	}

	for(moveFront(l); index(l)>=0; moveNext(l)){
		fputs(inputLines[get(l)], out);
	}
	fclose(in);
	fclose(out);
	freeList(&l);
	return 0;
}
	
		
