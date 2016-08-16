#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <stdlib.h>
#include <stdio.h>

struct node{
	struct node *next;
	int data;
};
typedef struct node Node;

typedef struct linked_list{
	Node *head;
	int load;
}LinkedList;

void Init(LinkedList *list);

LinkedList *append(LinkedList *list, int data);

LinkedList* del(LinkedList *list, int index);

int valueAt(LinkedList *list, int index);

LinkedList* insert(LinkedList *list, int index, int data);

LinkedList* removeItemVal(LinkedList *list, int data);

int isEmpty(LinkedList *list);

int size(LinkedList *list);

LinkedList* addStream(LinkedList *list, int data[]);

void print(LinkedList *list);

LinkedList* sort(LinkedList *list);

#endif // !LinkedList
