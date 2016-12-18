// Ric Rodriguez
// CMPS 101 -PA5
// FinComponents.c

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Usage: FindPath file1 file2\n");
		exit(1);
	}
	FILE* in;
	in =fopen(argv[1], "r");
	FILE* out; 
	out = fopen(argv[2], "w");
	if (in == NULL || out == NULL) {
		printf("Error: FindPath: main: could not open file\n");
		exit(1);
	}
	int vertices;
	fscanf(in, "%d", &vertices);
	Graph G = newGraph(vertices);
	for (;;) {
		int a, b;
		fscanf(in, "%d %d", &a, &b);
		if ((a == 0)&&(b==0)) {
			break;
		}
		addArc(G, a, b);
	}
	fprintf(out, "Adjacency list representation of G:\n");
	printGraph(out, G);
	List S = newList();
	for(int i =1;i<=getOrder(G);i++){
		append(S, i);
	}
	DFS(G, S);
	Graph H = transpose(G);
	DFS(H, S);
	
	List T = newList();
	// int *componentLens;
	// componentLens = calloc(1024, sizeof(*(componentLens)));
	int indx = 0;
	int components = 0;
	// see how many tree roots we have in our DFS forest
	for(moveFront(S); index(S) >= 0; moveNext(S)){
	        // append(T, get(S));
		// componentLens[components] = ++indx;
		if(getParent(H, get(S)) <= 0 ){ components++;}
		// if(getParent(G, get(S)) == NIL) components++;
		
	}
	//printf("%d components\n", components);
	
	fprintf(out, "\nG contains %d strongly connected components:\n", components);
	// print the trees. since dfs has them in reverse we start at the back of the list
	// until we hit the roots of the trees
	for(moveBack(S); index(S)>=0; movePrev(S)){
		prepend(T, get(S));
		if(getParent(H, get(S)) == NIL){
			fprintf(out, "Component %d: ", ++indx);
			printList(out, T);
			fprintf(out, "\n");
			clear(T);
		}
	}
		
	/* for(int i = 1; i <=components; i++){
		fprintf(out, "Component %d: ", i);
		for(int j = 0; j < sizeof(componentLens)/sizeof(componentLens[0]); j++){
			fprintf(out,"%d ", get(T));
			moveNext(T);
		}
		fprintf(out, "\n");
	}
	printList(stdout, T);
	*/
	fclose(out);
	fclose(in);
	freeList(&S);
	freeList(&T);
	freeGraph(&G);
	freeGraph(&H);
	return(0);
}
