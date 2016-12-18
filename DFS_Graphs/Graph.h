// Ric Rodriguez
// CMPS101-PA5
// Graph.h
#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

/*** Macros ***/
#define INF -1
#define NIL 0
#define WHITE 0
#define GREY 1
#define BLACK 2
#define UNDEF -1
/*** Information hiding of GraphObj ***/
typedef struct GraphObj* Graph;

/*** Contructors-Destructors ***/

// returns pointer to new graph
Graph newGraph(int n);

// Frees associated memory with graphobj
// takes pointer to graph as input
void freeGraph(Graph *pG);

/*** Access functions ***/

// returns order of graph
// @params Graph G
// @return number of vertices of graph as integer = |V|
int getOrder(Graph G);

// returns size of graph g
// @params graph g
// @return number of edges in g = 2|E| if undirected, |E| if directed
int getSize(Graph G);

// returns the parent of a vertex
// @params Graph g, vertex u
// @return vertex v, the parent of vertex u
int getParent(Graph G, int u);

// returns the discovery time of vertex u
// @params Graph G, Vertex U
// @return discovery time of U
int getDiscover(Graph G, int u);

// returns the finish time of vertex u
// @params Graph G, Vertex U
// @return finish time of U
int getFinish(Graph G, int u);

// adds edge to graph g connecting vertex u to vertex v
// @oarams graph g, parent vertex u, child vertex v
void addEdge(Graph G, int u, int v);

// adds directed edge with source vetex u and destination vertex v
// @params graph g, source vertex u, destination vertex v
void addArc(Graph G, int u, int v);

// performs breadth first search with source vertex s
// @params graph g, list of vertices s
void DFS(Graph G, List S);

/*** Other operations ***/
Graph transpose(Graph G);

Graph copyGraph(Graph G);
// prints the adjacency list representation of graph g
// @params FILE* out the stream to which we output the adjacency list
//     also graph g, the graph we wish to print
void printGraph(FILE* out, Graph G);

#endif // !__GRAPH_H__
