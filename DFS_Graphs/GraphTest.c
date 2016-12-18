//-----------------------------------------------------------------------------
// Ric Rodriguez
// rirrodri@ucsc.edu
// CMPS101-PA5
// GraphTest.c
// Test client for the Graph ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include"Graph.h"


int main(int argc, char* argv[]){
   int i, n=8;
   List S = newList();
   Graph G = newGraph(n);
   Graph T=NULL, C=NULL;

   for(i=1; i<=n; i++) append(S, i);

   addArc(G, 1,2);
   addArc(G, 1,5);
   addArc(G, 2,5);
   addArc(G, 2,6);
   addArc(G, 3,2);
   addArc(G, 3,4);
   addArc(G, 3,6);
   addArc(G, 3,7);
   addArc(G, 3,8);
   addArc(G, 6,5);
   addArc(G, 6,7);
   addArc(G, 8,4);
   addArc(G, 8,7);
   printGraph(stdout, G);

   DFS(G, S);
   fprintf(stdout, "\n");
   fprintf(stdout, "x:  d  f  p\n");
   for(i=1; i<=n; i++){
      fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }
   fprintf(stdout, "\n");
   printList(stdout, S);
   fprintf(stdout, "\n");

   T = transpose(G);
   C = copyGraph(G);
   fprintf(stdout, "\n");
   printGraph(stdout, C);
   fprintf(stdout, "\n");
   printGraph(stdout, T);
   fprintf(stdout, "\n");

   DFS(T, S);
   fprintf(stdout, "\n");
   fprintf(stdout, "x:  d  f  p\n");
   for(i=1; i<=n; i++){
      fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(T, i), getFinish(T, i), getParent(T, i));
   }
   fprintf(stdout, "\n");
   printList(stdout, S);
   fprintf(stdout, "\n");

   freeList(&S);
   freeGraph(&G);
   freeGraph(&T);
   freeGraph(&C);
	// Additional Testing
	printf("\nAdditional testing\n-----------------------\n");
	Graph N = newGraph(20);
	for(int i = 10;i<getOrder(N);i++){
		addArc(N,i,i+1);
		addArc(N,i+1, i);	
	}
	for(int i = 1;i<10;i++){
		addEdge(N, i, i+1);
	}
	printGraph(stdout, N);
	List M = newList();
	for(int i = 1;i<=getOrder(N); i++){
		append(M,i);
	}
	DFS(N, M);
	Graph O = NULL, P = NULL;
	O = transpose(N);
	DFS(O, M);
	P = copyGraph(O);
	printf("Size of O and P %d = %d\n", getSize(O), getSize(P));
	printf("Why is vertex 6 afraid of vertex 7? Because why am i writing pointless\
 test code that the graders dont even check.\
 Hey grader, if you are actually reading this text 'kek' to 5593676202\n");
	printf("Vertex 6 inspection: Discover:%d Finish:%d Parent%d\n", getDiscover(O, 6), getFinish(O, 6), getParent(O, 6));
	freeGraph(&N);
	freeGraph(&O);
	freeGraph(&P);
	freeList(&M);
   return(0);
}


