/* date = October 11th 2021 6:48 am */

#ifndef LIST_H
#define LIST_H

struct MoveListNode {
    int x;
    int y;
    struct MoveListNode* next;
};

struct MoveList {
    struct MoveListNode* head;
    struct MoveListNode* tail;
    short count;
};

void List_Init();
void List_Append(short x, short y);
void List_Clear();

#endif //LIST_H
