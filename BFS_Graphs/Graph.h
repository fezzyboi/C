// Ric Rodriguez
// CMPS101-PA4
// Graph.h
#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

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

// returns the most recent source of BFS
// @params graph g
// returns integer s, the most recent source that BFS was called with
int getSource(Graph G);

// returns the parent of a vertex
// @params Graph g, vertex u
// @return vertex v, the parent of vertex u
int getParent(Graph G, int u);

// returns the distance from source vertex to specified vertex u
// @params graph g, vertex u
// @return integer x, the distance between the most recent source vetex, and u
int getDist(Graph G, int u);

// returns the path of vertices from most recent source to u
// @params graph g, list L as list to store path, and int u as destination vertex
// @return shortest path from source s to vertex u
void getPath(List L, Graph G, int u);

/*** Manipulation procedures ***/

// returns graph g to its default state
// @params graph g
void makeNull(Graph G);

// adds edge to graph g connecting vertex u to vertex v
// @oarams graph g, parent vertex u, child vertex v
void addEdge(Graph G, int u, int v);

// adds directed edge with source vetex u and destination vertex v
// @params graph g, source vertex u, destination vertex v
void addArc(Graph G, int u, int v);

// performs breadth first search with source vertex s
// @params graph g, source vertex s
void BFS(Graph G, int s);

/*** Other operations ***/

// prints the adjacency list representation of graph g
// @params FILE* out the stream to which we output the adjacency list
//     also graph g, the graph we wish to print
void printGraph(FILE* out, Graph G);

#endif // !__GRAPH_H__
