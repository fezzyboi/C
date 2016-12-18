//------------------------------------------------------------------------
// List.c
// Implementation file for List ADT
// Ric Rodriguez
// rirrodri@ucsc.edu
// CMPS101-PA4
//------------------------------------------------------------------------

#include <stdio.h>
#include "List.h"
#include <stdlib.h>

// Structures ------------------------------------------------------------

// Private NodeObj type
typedef struct NodeObj
{
	struct NodeObj* next;
	struct NodeObj* prev;
	int data;
} NodeObj;

// Private Node type
typedef NodeObj* Node;

// Private ListObj type
typedef struct ListObj
{
	Node front;
	Node back;
	Node cursor;
	int index;
	int length;
}ListObj;

// Constructors-Destructors ----------------------------------------------

// newNode()
// Returns noderef to new obj
// Initialized next and data fields
// Private
Node newNode(int data, Node next, Node prev)
{
	Node N = malloc(sizeof(NodeObj));
	N->next = next;
	N->prev = prev;
	N->data = data;
	return N;
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL
// Private
void freeNode(Node* pN)
{
	if (pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

// newList()
// Returns ref to new ListObj
List newList()
{
	List list;
	list = malloc(sizeof(ListObj));
	list->back = NULL;
	list->front = NULL;
	list->cursor = NULL;
	list->length = 0;
	list->index = -1;
	return list;
}

// freeList()
// Frees all heap memory associated with list *pL, and sets *pL to NULL
void freeList(List* pL)
{
	if (pL != NULL && *pL != NULL) {
		Node itor = (*pL)->front;
		while (itor != NULL) {
			Node toFree = itor;
			itor = itor->next;
			free(toFree);
		}
		free(*pL);
		*pL = NULL;
	}
}

// Access functions ------------------------------------------------------
int isEmpty(List L)
{
	if (L == NULL) {
		printf("Error: List.c: isEmpty(): Unitialized List");
		exit(1);
	}
	return (length(L) == 0);
}


int length(List L)
{
	if (L == NULL) {
		printf("Error: List.c: length(): Uninitialized list");
		exit(1);
	}
	return L->length;
}

int index(List L)
{
	if (L == NULL) {
		printf("Error: List.c: index(): Uninitialized list");
		exit(1);
	}
	return L->index;
}

int front(List L)
{
	if (L == NULL) {
		printf("Error: List.c: front(): Uninitialized list");
		exit(1);
	}
	if (L->front == NULL || L->length <= 0) {
		printf("Error: List.c: front(): front is empty");
		exit(1);
	}
	return L->front->data;
}

int back(List L)
{
	if (L == NULL) {
		printf("Error: List.c: back(): Uninitialized list");
		exit(1);
	}
	if (L->back == NULL || L->length <= 0) {
		printf("Error: List.c: back(): back is empty");
		exit(1);
	}
	return L->back->data;
}

int get(List L)
{
	if (L == NULL) {
		printf("Error: List.c: back(): Uninitialized list");
		exit(1);
	}
	if (L->index<0 || L->length<1) {
		printf("Error: List.c: get(): cursor is undefined");
		exit(1);
	}
	return L->cursor->data;
}

int equals(List A, List B)
{
	if (A == NULL || B == NULL) {
		printf("Error: List.c: equals(): one or more uninitialized lists");
		exit(1);
	}
	if (length(A) != length(B)) return 0;
	Node testA = A->front;
	Node testB = B->front;
	for (int i = 0; i < length(A); i++) {
		if (testA->data != testB->data) return 0;
		testA = testA->next;
		testB = testB->next;
	}
	return 1;
}

// Manipulation Procedures -----------------------------------------------

void clear(List L)
{
	if (L == NULL) {
		printf("Error: List.c: clear(): Uninitialized list");
		exit(1);
	}
	Node itor = L->front;
	while (itor != NULL) {
		Node toFree = itor;
		itor = itor->next;
		free(toFree);
	}
	L->front = L->back = L->cursor = NULL;
	L->length = 0;
	L->index = -1;
}

void moveFront(List L)
{
	if (L == NULL) {
		printf("Error: List.c: moveFront(): Uninitialized list");
		exit(1);
	}
	if (L->length <= 0) return;
	L->cursor = L->front;
	L->index = 0;
}

void moveBack(List L)
{
	if (L == NULL) {
		printf("Error: List.c: moveBack(): Uninitialized list");
		exit(1);
	}
	if (L->length <= 0) return;
	L->cursor = L->back;
	L->index = L->length - 1;
}

void movePrev(List L)
{
	if (L == NULL) {
		printf("Error: List.c: movePrev(): Uninitialized list");
		exit(1);
	}
	if (L->index != 0 && L->cursor != NULL) {
		L->cursor = L->cursor->prev;
		L->index--;
		return;
	}
	if (L->index == 0 && L->cursor != NULL) {
		L->cursor = NULL;
		L->index = -1;
		return;
	}
}


void moveNext(List L)
{
	if (L == NULL) {
		printf("Error: List.c: moveNext(): Uninitialized list");
		exit(1);
	}
	if (L->cursor != L->back && L->cursor != NULL) {
		L->cursor = L->cursor->next;
		L->index++;
		return;
	}
	if (L->cursor == L->back && L->cursor != NULL) {
		L->cursor = NULL;
		L->index = -1;
		return;
	}
}

void prepend(List L, int data)
{
	Node N = newNode(data, L->front, NULL);
	if (L->front == NULL) {
		L->back = N;
	}
	else {
		L->front->prev = N;
	}
	L->front = N;
	L->length++;
}

void append(List L, int data)
{
	Node N = newNode(data, NULL, L->back);
	if (L->front == NULL) {
		L->front = N;
	}
	else {
		L->back->next = N;
	}
	L->back = N;
	L->length++;
}

void insertBefore(List L, int data)
{
	if (L->length <= 0 || L->index < 0) {
		return;
	}
	Node N = newNode(data, L->cursor, L->cursor->prev);
	if (L->cursor->prev != NULL) {
		L->cursor->prev->next = N;
	}
	else {
		L->front = N;
	}
	L->cursor->prev = N;
	L->length++;
}

void insertAfter(List L, int data)
{
	if (L->length<1 || L->index <0) {
		return;
	}
	Node N = newNode(data, L->cursor->next, L->cursor);
	if (L->cursor->next != NULL) {
		L->cursor->next->prev = N;
	}
	else {
		L->back = N;
	}
	L->cursor->next = N;
	L->length++;
}

void deleteFront(List L)
{
	if (L->length <= 0) {
		return;
	}
	if (L->cursor == L->front) {
		L->cursor = NULL;
		L->index = -1;
	}
	Node toFree = L->front;

	if(L->front == L->back){
		L->back = NULL;
	}
	if(L->front->next != NULL){
		L->front = L->front->next;
	}
	L->front->prev = NULL;
	L->length--;
	freeNode(&toFree);
}

void deleteBack(List L)
{
	if (L->length <= 0) {
		return;
	
	}
	if (L->cursor == L->back) {
		L->cursor = NULL;
		L->index = -1;
	}
	Node tofree = L->back;

	if (L->back == L->front) {
		L->front = NULL;
	}
	if (L->back->prev != NULL) {
		L->back = L->back->prev;
	}
	L->back->next = NULL;
	L->length--;
	freeNode(&tofree);
}

void delete(List L)
{
	if (L->length <= 0 || L->index <0) {
		return;
	}
	if (L->cursor == L->front) deleteFront(L);
	if (L->cursor == L->back) deleteBack(L);
	Node toFree = L->cursor;
	L->cursor->prev->next = L->cursor->next;
	L->cursor->next->prev = L->cursor->prev;
	free(toFree);
	L->cursor = NULL;
	L->index = -1;
	L->index--;
	L->length--;

}

void printList(FILE* out, List L)
{
	if (out == NULL) {
		printf("Error: List.c: printList(): File is null\n");
		exit(1);
	}
	moveFront(L);
	for (int i = 0; i<length(L); i++) {
		fprintf(out, "%d ", get(L));
		moveNext(L);
	}
}

List copyList(List L)
{
	List list = newList();
	for (moveFront(L); L->index >= 0; moveNext(L)) {
		append(list, get(L));
	}
	return list;
}
