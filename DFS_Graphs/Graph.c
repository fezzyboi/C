// Ric Rodriguez
// CMPS-101-PA4
// Graph.c

#include "Graph.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

// Structure for Graph 
typedef struct GraphObj
{
	List* neighbors;
	int* colors;
	int* parents;
	int* discover;
	int* finish;
	int order;
	int size;
}GraphObj;

static void DFS_VISIT(Graph G, List S, int u, int* time);

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
	new->finish = (int*)calloc(np, sizeof(int));
	new->discover = (int*)calloc(np, sizeof(int));
	new->neighbors = (List*)calloc(np, sizeof(List));
	new->size = 0;
	new->order = n;
	for (int i = 0; i < np; ++i) {
		new->neighbors[i] = newList();
		new->colors[i] = WHITE;
		new->discover[i] = UNDEF;
		new->finish[i] = UNDEF;
		new->parents[i] = NIL;
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
	free((*pG)->discover);
	free((*pG)->finish);
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

	return G->parents[u] !=NIL ? G->parents[u] : NIL;
}

// returns the discover time of a vertex
// @params Graph g, vertex u
// @return integer representation of vertex time discovery
int getDiscover(Graph G, int u)
{
	if (G==NULL){
		printf("Error: Graph.c: getDiscover: Uninitialized Graph\n");
		exit(1);
	}

	if (u < 1 || u > getOrder(G)){
		printf("Error: Graph.c: getDiscover: Precondition: 1<=u<=getOrder(G)\n");
		exit(1);
	}
	return G->discover[u] >0?G->discover[u]:NIL;
}

// returns the finish time of a vertex
// @params Graph g, vertex u
// @return integer representation of vertex time finish
int getFinish(Graph G, int u)
{
	if (G==NULL){
		printf("Error: Graph.c: getFinish: Uninitialized Graph\n");
		exit(1);
	}

	if (u < 1 || u > getOrder(G)){
		printf("Error: Graph.c: getFinish: Precondition: 1<=u<=getOrder(G)\n");
		exit(1);
	}
	return G->finish[u] >0?G->finish[u]:UNDEF;
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
void DFS(Graph G, List S)
{
	if (G == NULL || S == NULL) {
		printf("Error: Graph.c: DFS: Uninitialized Graph\n");
		exit(1);
	}

	if(getOrder(G)!=length(S)){
		printf("Error: Graph.c: DFS: List must be of same order as G\n");
		exit(1);
	}

	for(int i = 0;i<=getOrder(G); ++i){
		G->colors[i] = WHITE;
		G->parents[i] = NIL;
		G->discover[i] = NIL;
		G->finish[i] = UNDEF;
	}

	int time = 0;
	moveFront(S);
	while(index(S)>=0){
		if(G->colors[get(S)] == WHITE){
			DFS_VISIT(G, S, get(S), &time);
		}
		moveNext(S);
	}

	for(int i = length(S)/2;i>0;i--){
		deleteBack(S);
	}
}

// Performs depth first search visit for given graph g
// @params Graph g, list s, vertex u, and address of time variable
// @return nothing lol
static void DFS_VISIT(Graph G,List S, int u, int* time){
	(*time)++;
	G->discover[u] = *time;
	G->colors[u] = GREY;
	moveFront(G->neighbors[u]);
	while(index(G->neighbors[u])>=0){
		if(G->colors[get(G->neighbors[u])]==WHITE){
			G->parents[get(G->neighbors[u])] = u;
			DFS_VISIT(G, S, get(G->neighbors[u]), time);
		}
		moveNext(G->neighbors[u]);
	}
	G->colors[u] = BLACK;
	(*time)++;
	G->finish[u] = *time;
	prepend(S, u);
}

// Reverses direcctions of edges of G
// @params Graph g
// @return transpose of g
Graph transpose(Graph G)
{
	if (G == NULL) {
		printf("Error: Graph.c: transpose: Uninitialized Graph\n");
		exit(1);
	}

	Graph new = newGraph(getOrder(G));

	for(int i = 0;i<=getOrder(G);++i){
		moveFront(G->neighbors[i]);
		while(index(G->neighbors[i])>=0){
			addArc(new, get(G->neighbors[i]), i);
			moveNext(G->neighbors[i]);
		}
	}
	return new;
}

// copies graph
// @params takes graph g 
// @return graph g as a pointer to a newly malloc'ed graph
Graph copyGraph(Graph G)
{
	if (G == NULL) {
		printf("Error: Graph.c: transpose: Uninitialized Graph\n");
		exit(1);
	}

	Graph new = newGraph(getOrder(G));

	for(int i = 0;i<=getOrder(G);++i){
		moveFront(G->neighbors[i]);
		while(index(G->neighbors[i])>=0){
			addArc(new, i, get(G->neighbors[i]));
			moveNext(G->neighbors[i]);
		}
	}
	return new;
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
