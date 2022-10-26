#ifndef __TOUCH_H__
#define __TOUCH_H__

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

int get_ts(int *x, int *y);

#endif