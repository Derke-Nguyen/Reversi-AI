#include "list.h"
#include <stddef.h>
#include <stdlib.h>

void List_Init(MoveList** list) {
    *list = malloc(sizeof(MoveList));
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->count = 0;
}

MoveListNode* List_GetMove(MoveList* list, short index) {
    MoveListNode* temp = list->head;
    for(int i = 0; i < index; ++i) {
        temp = temp->next;
    }
    
    return temp;
}

void List_Append(MoveList* list, short x, short y) {
    MoveListNode* newnode = malloc(sizeof(MoveListNode));
    newnode->x = x;
    newnode->y = y;
    newnode->next = NULL;
    
    //if not empty
    if(list->count != 0) {
        list->tail->next = newnode;
        list->tail = newnode;
        list->count += 1;
    }
    //if the list is empty
    else {
        list->head = newnode;
        list->tail = newnode;
        list->count = 1;
    }
    
}

void List_Clear(MoveList* list) {
    MoveListNode* currnode = list->head;
    MoveListNode* nextnode = NULL;
    while(currnode != NULL) {
        nextnode = currnode->next;
        free(currnode);
        currnode = nextnode;
    }
    
    
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}