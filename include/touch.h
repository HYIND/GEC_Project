#ifndef _TOUCH_H
#define _TOUCH_H

#include "fd.h"
#include <linux/input.h>

struct points
{
    int x;
    int y;
};

//定义一个结构体存放读取到的触摸屏信息
extern struct input_event touch;

extern struct points P_I;

void Get_Touch_Data();

#endif