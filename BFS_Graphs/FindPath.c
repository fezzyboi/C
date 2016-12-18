// Ric Rodriguez
// CMPS 101 -PA4
// FindPath.c

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
		addEdge(G, a, b);
	}
	printGraph(out, G);
	int x = 0;
	for (;;) {
		int a, b;
		fscanf(in, "%d %d", &a, &b);
		if ((a == 0) && (b == 0)) {
			break;
		}
		BFS(G, a);
		List path = newList();
		getPath(path, G, b);
		if (x == 0) {
			fprintf(out, "\n");
			x++;
		}
		else {
			fprintf(out, "\n");
		}
		if(getDist(G,b)>=0){
			fprintf(out, "The distance from %d to %d is %d\n", a, b, getDist(G, b));
			fprintf(out, "A shortest %d-%d path is: ", a, b);
			printList(out, path);
			
			
		}else{
			fprintf(out, "The distance from %d to %d is %s\n", a, b, "infinity");
			fprintf(out, "No %d-%d path exists\n", a, b);
		}
		freeList(&path);
		fprintf(out, "\n");
	}
	fclose(out);
	fclose(in);
	freeGraph(&G);
	return 0;
}
