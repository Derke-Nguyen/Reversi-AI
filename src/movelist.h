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
*       List datastructure to store moves possible for AI
*       Not cache friendly, but only iterating through once, so it's fine
*
********************************************************************************************/

#ifndef LIST_H
#define LIST_H

#include "def.h"

typedef struct MoveListNode {
    int8_t x;
    int8_t y;
    float score;
    struct MoveListNode* next;
} MoveListNode;

typedef struct MoveList {
    struct MoveListNode* head;
    struct MoveListNode* tail;
    short count;
} MoveList;

// Initialize list to use
// list: list to initialize
bool List_Init(MoveList** list);

// Gets the move with the highest score
// list: list to iterate through
// @Return the node with the highest score
MoveListNode* List_GetMax(MoveList* list);

// Adds a new node to the end of the list
// list: the list to add to
// x: x position of move
// y: y position of move
// score: the score generated from the move
// @Return true if player turn matches player
void List_Append(MoveList* list, int8_t x, int8_t y, float score);

// Frees all the nodes and empties the list
// list: list to clear
void List_Clear(MoveList* list);

#endif //LIST_H
