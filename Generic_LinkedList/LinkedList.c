#include "LinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

LinkedList* append(LinkedList *list, int data)
{
	Node *insert = malloc(sizeof(Node));
	insert->data = data;
	insert->next = NULL;
	Node *curr;
	curr = list->head;

	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = insert;
	list->load++;
	return list;
}

LinkedList* del(LinkedList *list, int index)
{
	if (list == NULL) {
		return list;
	}
	Node *curr = list->head;
	printf("index is: %d\n", index);
	if (index == 0) {
		if (list->load == 0)
			return NULL;
		Node *tmp = list->head;
		list->head = list->head->next;
		free(tmp);
		return list;
	}
	for (int i = 0; i < index; i++) {
		curr = curr->next;
	}
	if (curr == NULL) {
		printf("Invalid Index\n");
		return list;
	}
	Node *tmp = curr->next;
	curr->next = tmp->next;
	free(tmp);
	list->load--;
	return list;
}

int valueAt(LinkedList *list, int index)
{
	if (list == NULL) {
		return 0;
	}
	Node *curr = list->head;
	while (index-- > 0 && curr != NULL) {
		curr = curr->next;
	}
	return curr->data;
}

LinkedList* insert(LinkedList *list, int index, int data)
{
	Node *curr = list->head;
	while (--index > 0 && curr->next != NULL) {
		curr = curr->next;
	}
	Node *in = malloc(sizeof(Node));
	in->data = data;
	in->next = curr->next;
	curr->next = in;
	list->load++;
	return list;
}


LinkedList* removeItemVal(LinkedList *list, int data)
{
	if (list == NULL) {
		return NULL;
	}
	Node *curr = list->head;
	while (curr->next->data != data) {
		curr = curr->next;
	}
	Node *tmp = curr->next;
	curr->next = curr->next->next;
	free(tmp);
	list->load--;
	return list;
}

int isEmpty(LinkedList *list)
{
	if (list->head == NULL) {
		return 1;
	}
	return list->load == 0 ? 1 : 0;
}

int size(LinkedList *list)
{
	if (list->head == NULL) {
		return 0;
	}
	return list->load;
}
void Init(LinkedList *list)
{
	list->head = NULL;
	list->load = 0;
}
LinkedList	*addStream(LinkedList *list, int data[])
{
	size_t len = (sizeof(data) * sizeof(data[0]));
	size_t i = 0;
	Node *first = malloc(sizeof(Node));
	first->data = data[i++];
	list->head = first;
	list->load++;
	Node *curr = list->head;
	while (i < len) {
		Node *in = malloc(sizeof(Node));		
		in->data = data[i++];
		curr->next = in;
		list->load++;
		curr = curr->next;
	}
	curr->next = NULL;
	return list;
}

void print(LinkedList *list)
{
	if (list->head == NULL) {
		return;
	}
	Node *curr = list->head;
	printf("\nPrinting linked list:\n[");
	while (curr->next!=NULL) {
		printf("%d -> ", curr->data);
		curr = curr->next;
	}
	printf("%d]\n", curr->data);
	printf("\nDone printing list\n");
}

LinkedList* sort(LinkedList *list)
{
	if (list->head == NULL) {
		return list;
	}
	Node *curr = list->head;
	int i = list->load;
	for(int i=0;i<list->load;i++){
		for(int j=1;j<list->load;j++){
			int cmp1 = curr->data;
			int cmp2 = curr->next->data;
			if (cmp1 > cmp2) {
				int tmp = curr->data;
				curr->data = curr->next->data;
				curr->next->data = tmp;
			}
			curr = curr->next;
		}
		curr = list->head;
	}
	return list;
}
