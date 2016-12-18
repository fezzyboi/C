//-----------------------------------------------------------------------------
// Ric Rodriguez
// GraphTest.c
// CMPS101-PA4
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main(int argc, char* argv[])
{
	int i, s, max, min, d, n = 35;
	List  C = newList(); // central vertices 
	List  P = newList(); // peripheral vertices 
	List  E = newList(); // eccentricities 
	Graph G = NULL;

	// Build graph G 
	G = newGraph(n);
	for (i = 1; i<n; i++) {
		if (i % 7 != 0) addEdge(G, i, i + 1);
		if (i <= 28) addEdge(G, i, i + 7);
	}
	addEdge(G, 9, 31);
	addEdge(G, 17, 13);
	addEdge(G, 14, 33);

	// Print adjacency list representation of G

	printGraph(stdout, G);
	// Calculate the eccentricity of each vertex 
	for (s = 1; s <= n; s++) {

		BFS(G, s);
		max = getDist(G, 1);
		for (i = 2; i <= n; i++) {
			d = getDist(G, i);
			max = (max<d ? d : max);
		}
		append(E, max);
	}
	// Determine the Radius and Diameter of G, as well as the Central and 
	// Peripheral vertices.
	append(C, 1);
	append(P, 1);
	min = max = front(E);
	moveFront(E);
	moveNext(E);
	for (i = 2; i <= n; i++) {
		d = get(E);
		if (d == min) {
			append(C, i);
		}
		else if (d<min) {
			min = d;
			clear(C);
			append(C, i);
		}
		if (d == max) {
			append(P, i);
		}
		else if (d>max) {
			max = d;
			clear(P);
			append(P, i);
		}
		moveNext(E);
	}

	// Print results 
	printf("\n");
	printf("Radius = %d\n", min);
	printf("Central vert%s: ", length(C) == 1 ? "ex" : "ices");
	printList(stdout, C);
	printf("\n");
	printf("Diameter = %d\n", max);
	printf("Peripheral vert%s: ", length(P) == 1 ? "ex" : "ices");
	printList(stdout, P);
	printf("\n");

	// Free objects 
	freeList(&C);
	freeList(&P);
	freeList(&E);
	freeGraph(&G);

	// Additional testing
	int x = 100;
	int y = 100000;
	G = newGraph(x);
	Graph H = newGraph(y);
	for(int i = 2;i<x;i++){
		addEdge(G, i-1, i);
	}
	for(int i = 2;i<y-1;i++){
		addArc(H, i-1, i);
	}
	printf("Order of G: %d\nOrder of H: %d\n", getOrder(G), getOrder(H));
	printf("Size of G: %d\nSize of H: %d\n", getSize(G), getSize(H));
	BFS(G, 50);
	BFS(H,500);
	printf("Most recent source of G: %d\nMost recent source of H: %d\n", getSource(G), getSource(H));
	printf("Parent of some vertex: %d\nParent of some vertex: %d\n", getParent(G, 89), getParent(H, 890));
	printf("Distance from x to y in G: %d\nDistance from x to y in H: %d\n", getDist(G, 78), getDist(H, 551));
	printf("Path from 50 to 78 in G: ");
	List L = newList();
	getPath(L, G, 78);
	printList(stdout, L);
	printf("\nThis should be -1: %d\n",	getDist(H, 1));
	
	freeList(&L);
	List M = newList();
	getPath(M, H, 1);
	printList(stdout, M);
	printf("\nPrinting lists now\n");
	freeList(&M);
	printGraph(stdout, G);
	printGraph(stdout, H);
	freeGraph(&G);
	freeGraph(&H);

	return(0);
}
