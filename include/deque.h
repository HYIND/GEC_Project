#ifndef __DEQUE_H__
#define __DEQUE_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef char *data_type;

typedef struct listNode
{
    /* 数据域 */
    data_type Data;

    struct listNode *prev; //前驱指针
    struct listNode *next; //前驱指针
} Node, *P_Node;

#endif

P_Node New_Node(data_type path);

int List_Add_To(P_Node head, data_type path, P_Node new);

int Display_List(P_Node head, bool flag);