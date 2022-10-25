#ifndef __ALBUM_H__
#define __ALBUM_H__

#include "touch.h"
#include <stdlib.h>
#include <stdbool.h>

typedef char* data_type;

typedef struct listNode
{
    /* 数据域 */
    data_type Data;

    struct listNode *prev; //前驱指针
    struct listNode *next; //前驱指针
} Node, *P_Node;

void Album();

#endif