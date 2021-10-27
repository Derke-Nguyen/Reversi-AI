/*******************************************************************************************
*
*   Reversi AI
*
*   This game has been created using raylib 3.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Derek Nguyen
*
*   DESCRIPTION:
*       Definitions of list functions
*
********************************************************************************************/

#include "movelist.h"

bool List_Init(MoveList** list) {
    *list = malloc(sizeof(MoveList));
    if (*list == NULL) {
        printf("ERROR: GAME: Failed to initialize movelist\n");
        return false;
    }
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->count = 0;
    return true;
}

MoveListNode* List_GetMax(MoveList* list) {
    MoveListNode* currnode = list->head;
    MoveListNode* maxnode = currnode;
    int maxscore = currnode->score;
    while (currnode != NULL) {
        if (currnode->score > maxscore) {
            maxscore = currnode->score;
            maxnode = currnode;
        }
        currnode = currnode->next;
    }
    return maxnode;
}

void List_Append(MoveList* list, int8_t x, int8_t y, float score) {
    MoveListNode* newnode = malloc(sizeof(MoveListNode));
    if (newnode == NULL) {
        printf("ERROR: GAME: Failed to add a new node to movelist\n");
        return;
    }
    newnode->x = x;
    newnode->y = y;
    newnode->score = score;
    newnode->next = NULL;
    
    // if not empty
    if(list->count != 0) {
        list->tail->next = newnode;
        list->tail = newnode;
        list->count += 1;
    }
    // if the list is empty
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