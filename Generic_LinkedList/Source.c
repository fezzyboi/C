#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "LinkedList.h"
int main(void)
{
	int values[] = { 4, 2, 45, 1, 0, 123, -90, -12, 123 , 612, 512, 9012, 10292, -9293, 0, 2939 };
	LinkedList *ll=malloc(sizeof(LinkedList));
	Init(ll);
	addStream(ll, values);
	print(ll);
	sort(ll);
	print(ll);
	append(ll, 500);
	print(ll);
	insert(ll, 2, 200);
	print(ll);
	del(ll, 5);
	print(ll);
	removeItemVal(ll, 512);
	print(ll);
	for (int i = 0; i < size(ll); i++) {
		printf("i is %d\n", i);
		del(ll, 0);
	}
	print(ll);

	getchar();
	return EXIT_SUCCESS;
}
