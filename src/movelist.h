/* date = October 11th 2021 6:48 am */

#ifndef LIST_H
#define LIST_H

typedef struct MoveListNode {
    short x;
    short y;
    float score;
    struct MoveListNode* next;
} MoveListNode;

typedef struct MoveList {
    struct MoveListNode* head;
    struct MoveListNode* tail;
    short count;
} MoveList;

void List_Init(MoveList** list);
MoveListNode* List_GetMove(MoveList* list, short index);
void List_Append(MoveList* list, short x, short y, float score);
void List_Clear(MoveList* list);
int List_FindMax(MoveList* list);
#endif //LIST_H
