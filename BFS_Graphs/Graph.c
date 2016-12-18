// Ric Rodriguez
// CMPS-101-PA4
// Graph.c

#include "Graph.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

/*** Macros ***/
#define INF -1
#define NIL 0
#define WHITE 0
#define GREY 1
#define BLACK 2


// Structure for Graph 
typedef struct GraphObj
{
	List* neighbors;
	int* colors;
	int* parents;
	int* dist;
	int order;
	int size;
	int mostRecentSource;
}GraphObj;

// Returns pointer to new graph with input of size n
Graph newGraph(int n)
{
	int np = n + 1;
	Graph new = malloc(sizeof(struct GraphObj));
	if (new == NULL) {
		printf("Error: Graph.c: newGraph: malloc failed\n");
		exit(1);
	}
	new->colors = (int*)calloc(np, sizeof(int));
	new->parents = (int*)calloc(np, sizeof(int));
	new->dist = (int*)calloc(np, sizeof(*(new->dist)));
	new->neighbors = (List*)calloc(np, sizeof(List));
	new->size = 0;
	new->order = n;
	new->mostRecentSource = NIL;
	for (int i = 0; i < np; ++i) {
		new->neighbors[i] = newList();
		new->colors[i] = WHITE;
		new->parents[i] = NIL;
		new->dist[i] = INF;
	}
	return new;
}

// Frees associated memory with graphobj
// takes pointer to graph as input
void freeGraph(Graph *pG)
{
	if (pG != NULL && *pG != NULL) {
		for (int i = 0; i <=(*pG)->order; ++i) {
			freeList(&((*pG)->neighbors[i]));
		}
	}
	free((*pG)->neighbors);
	free((*pG)->parents);
	free((*pG)->dist);
	free((*pG)->colors);
	free(*pG);
	*pG = NULL;
}

// returns order of graph
// @params Graph G
// @return number of vertices of graph as integer = |V|
int getOrder(Graph G)
{
	if (G == NULL) {
		printf("Error: Graph.c: getOrder: Uninitialized Graph\n");
		exit(1);
	}

	return G->order;
}

// returns size of graph g
// @params graph g
// @return number of edges in g = 2|E| if undirected, |E| if directed
int getSize(Graph G)
{
	if (G == NULL) {
		printf("Error: Graph.c: getSize: Uninitialized Graph\n");
		exit(1);
	}

	return G->size;
}

// returns the most recent source of BFS
// @params graph g
// returns integer s, the most recent source that BFS was called with
int getSource(Graph G)
{
	if (G == NULL) {
		printf("Error: Graph.c: getSource: Uninitialized Graph\n");
		exit(1);
	}

	return G->mostRecentSource > 0 ? G->mostRecentSource : NIL;
}

// returns the parent of a vertex
// @params Graph g, vertex u
// @return vertex v, the parent of vertex u
int getParent(Graph G, int u)
{
	if (G == NULL) {
		printf("Error: Graph.c: getParent: Uninitialized Graph\n");
		exit(1);
	}

	if (u < 1 || u > getOrder(G)) {
		printf("Error: Graph.c: getParent: Precondition: 1<=u<=getOrder(G)\n");
		exit(1);
	}

	return G->parents[u] > 0 ? G->parents[u] : INF;
}

// returns the distance from source vertex to specified vertex u
// @params graph g, vertex u
// @return integer x, the distance between the most recent source vetex, and u
int getDist(Graph G, int u)
{
	if (G == NULL) {
		printf("Error: Graph.c: getDist: Uninitialized Graph\n");
		exit(1);
	}

	if (u < 1 || u > getOrder(G)) {
		printf("Error: Graph.c: getDist: Precondition: 1<=u<=getOrder(G)\n");
		exit(1);
	}
	if (G->mostRecentSource == u) {
		return 0;
	}
	return G->dist[u] > 0 ? G->dist[u] : INF;
}

// returns the path of vertices from most recent source to u 
// @params graph g, list L as list to store path, and int u as destination vertex
// @return shortest path from source s to vertex u
void getPath(List L, Graph G, int u)
{
	if (G == NULL) {
		printf("Error: Graph.c: getPath: Uninitialized Graph\n");
		exit(1);
	}

	if (getSource(G) == NIL) {
		printf("Error: Graph.c: getPath: Precondition: getSource(G)!=NIL\n");
		exit(1);
	}

	if (u < 1 || u > getOrder(G)) {
		printf("Error: Graph.c: getPath: Precondition: 1<=u<=getOrder(G)\n");
		exit(1);
	}

	if (getSource(G) == u) {
		append(L, u);
	}
	else if (G->parents[u] == NIL) {
		append(L, NIL);
	}
	else {
		getPath(L, G, G->parents[u]);
		append(L, u);
	}
}


// returns graph g to its default state
// @params graph g
void makeNull(Graph G)
{
	if (G == NULL) {
		printf("Error: Graph.c: makeNull: Uninitialized Graph\n");
		exit(1);
	}

	for (int i = 0; i <= G->order; ++i) {
		clear(G->neighbors[i]);
	}
	G->size = 0;
}
 
// adds edge to graph g connecting vertex u to vertex v
// @oarams graph g, parent vertex u, child vertex v
void addEdge(Graph G, int u, int v)
{
	if (G == NULL) {
		printf("Error: Graph.c: addEdge: Uninitialized Graph\n");
		exit(1);
	}
	if (u > getOrder(G) || u < 1 || v > getOrder(G) || v < 1) {
		printf("Error: Graph.c: addEdge: Precondition: 1<=(v,u)<=getOrder(G)\n");
		exit(1);
	}
	moveFront(G->neighbors[u]);
	while (index(G->neighbors[u]) > -1 && v > get(G->neighbors[u])) {
		moveNext(G->neighbors[u]);
	}
	if (index(G->neighbors[u]) < 0) {
		append(G->neighbors[u], v);
	}
	else {
		insertBefore(G->neighbors[u], v);
	}
	
	moveFront(G->neighbors[v]);
	while (index(G->neighbors[v]) > -1 && u > get(G->neighbors[v])) {
		moveNext(G->neighbors[v]);
	}
	if (index(G->neighbors[v]) < 0) {
		append(G->neighbors[v], u);
	}
	else {
		insertBefore(G->neighbors[v], u);
	}
	G->size = G->size + 2;

}

// adds directed edge with source vetex u and destination vertex v
// @params graph g, source vertex u, destination vertex v
void addArc(Graph G, int u, int v)
{
	if (G == NULL) {
		printf("Error: Graph.c: addArc: Uninitialized Graph\n");
		exit(1);
	}

	if (u > getOrder(G) || u < 1 || v > getOrder(G) || v < 1) {
		printf("Error: Graph.c: makeArc: Precondition: 1<=(v,u)<=getOrder(G)\n");
		exit(1);
	}

	moveFront(G->neighbors[u]);
	while (index(G->neighbors[u]) > -1 && v > get(G->neighbors[u])) {
		moveNext(G->neighbors[u]);
	}
	if (index(G->neighbors[u]) < 0) {
		append(G->neighbors[u], v);
	}
	else {
		insertBefore(G->neighbors[u], v);
	}
	G->size++;
	
}

// performs breadth first search with source vertex s
// @params graph g, source vertex s
void BFS(Graph G, int s)
{
	for (int i = 0; i <= getOrder(G); ++i) {
		G->colors[i] = WHITE;
		G->dist[i] = INF;
		G->parents[i] = NIL;
	}
	G->mostRecentSource = s;
	G->colors[s] = GREY;
	G->dist[s] = 0;
	G->parents[s] = NIL;
	List Q = newList();
	prepend(Q, s);
	while (length(Q) > 0) {
		int u = back(Q);
		deleteBack(Q);
		moveFront(G->neighbors[u]);
		while(index(G->neighbors[u]) >= 0) {
			int v = get(G->neighbors[u]);
			if (G->colors[v] == WHITE) {
				G->colors[v] = GREY;
				G->dist[v] = G->dist[u]+1;
				G->parents[v] = u;
				prepend(Q, v);
			}
			moveNext(G->neighbors[u]);
		}
		G->colors[u] = BLACK;
	}
	freeList(&Q);
}

// prints the adjacency list representation of graph g
// @params FILE* out the stream to which we output the adjacency list
//     also graph g, the graph we wish to print
void printGraph(FILE* out, Graph G)
{

	for (int i = 1; i <= getOrder(G); i++) {
		fprintf(out, "%d:", i);
		for (moveFront(G->neighbors[i]); index(G->neighbors[i]) >= 0; moveNext(G->neighbors[i])) {
			fprintf(out, " %d", get(G->neighbors[i]));
		}
		fprintf(out, "\n");
	}
}









