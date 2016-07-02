#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BOARD.h"

ListItem *LinkedListNew(char *data)
{
    ListItem *head = malloc(sizeof (ListItem));
    if (head == NULL) {
        return NULL;
    }
    head->previousItem = NULL;
    head->nextItem = NULL;
    head->data = data;
    return head;
}

char * LinkedListRemove(ListItem *item)
{
    if (item == NULL) {
        return NULL;
    }
    char *tmp = item->data;
    item=item->nextItem;
    free(item);
    return tmp;
}

int LinkedListSize(ListItem *list)
{

    // Check if list is null, if so return size 0
    if (list == NULL) {
        return 0;
    }

    // Recursively calculate size of list. Add one to each successful iteration
    return (1 + LinkedListSize(list->nextItem));
}

ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list == NULL) {
        return NULL;
    }
    ListItem *curr = list;
    while (curr->previousItem != NULL) {
        curr = curr->previousItem;
    }
    return curr;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    ListItem *new = malloc(sizeof (ListItem));
    new->data = data;
    if (item == NULL) {
        new->previousItem = NULL;
    } else {
        new->previousItem = item;
    }
    if (item->nextItem == NULL) {
        item->nextItem = new;
    } else {
        item->nextItem->previousItem = new;
        new->nextItem = item->nextItem;
        item->nextItem = new;
    }
    return new;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if ((firstItem != NULL && secondItem == NULL) || (firstItem == NULL && secondItem != NULL)) {
        return STANDARD_ERROR;
    }
    char *tmp = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = tmp;
    return SUCCESS;
}

int LinkedListSort(ListItem *list)
{

    // Check for null list
    if (list == NULL) {
        return STANDARD_ERROR;
    }

    // Sort by word length
    int i = 0;
    while (i < LinkedListSize(list) - 2) {
        while (list->nextItem != NULL) {
            int len1, len2;
            if (list->data == NULL) {
                len1 = 0;
            } else {
                len1 = strlen(list->data);
            }
            if (list->nextItem->data == NULL) {
                len2 = 0;
            } else {
                len2 = strlen(list->nextItem->data);
            }
            int cmp = len1 - len2;
            if (cmp > 0) {
                LinkedListSwapData(list, list->nextItem);
            }
            list = list->nextItem;
        }
        i++;
        list = LinkedListGetFirst(list);
    }
    list = LinkedListGetFirst(list);
    i = 0;
    while (i < LinkedListSize(list)) {
        while (list->nextItem != NULL) {
            int len1, len2;
            if (list->data == NULL) {
                len1 = 0;
            } else {
                len1 = strlen(list->data);
            }
            if (list->nextItem->data == NULL) {
                len2 = 0;
            } else {
                len2 = strlen(list->nextItem->data);
            }
            if (len1 == len2) {
                int cmp = strcmp(list->data, list->nextItem->data);
                if (cmp > 0) {
                    LinkedListSwapData(list, list->nextItem);
                }
            }
            list = list->nextItem;
        }
        i++;
        list = LinkedListGetFirst(list);
    }
    return SUCCESS;
}

int LinkedListPrint(ListItem *list)
{
    if (list == NULL) {
        return STANDARD_ERROR;
    }
    printf("{ ");
    while (list != NULL) {
        printf("%s ", list->data);
        list = list->nextItem;
    }
    printf("}\n");
    return SUCCESS;
}
