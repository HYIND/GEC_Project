#include "touch.h"
#include <unistd.h>

//定义一个结构体存放读取到的触摸屏信息
struct input_event touch;
struct points P_I;

void Get_Touch_Data()
{
    read(fd_ts, &touch, sizeof(touch));
    if (touch.type == EV_ABS && touch.code == ABS_X)
        P_I.x = touch.value; //判断是不是触摸屏设备，在判断是不是x坐标，
    if (touch.type == EV_ABS && touch.code == ABS_Y)
        P_I.y = touch.value;
}

int get_ts(int *x, int *y)
{
    int x_s = 0, y_s = 0, x_e = 0, y_e = 0;
    //循环获取触摸事件
    while (1)
    {
        //读取事件的数据
        read(fd_ts, &touch, sizeof(touch));
        //循环获取x轴坐标
        if (touch.type == EV_ABS && touch.code == ABS_X)
        {
            *x = touch.value; //保存x轴坐标
        }
        //循环获取y轴坐标
        if (touch.type == EV_ABS && touch.code == ABS_Y)
        {
            *y = touch.value; //保存y轴坐标
        }
        //判断触摸起点，保存起点位置坐标
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 1)
        {
            x_s = *x;
            y_s = *y;
        }
        //松手判断，保存终点坐标
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        {
            x_e = *x;
            y_e = *y;
            //判断始末坐标位置差距，优先左右划动，所以先判断x轴坐标
            if (x_e - x_s < 100 && x_e - x_s > -100)
            {
                if (y_e - y_s < 50 && y_e - y_s > -50)
                    return 0;
                else if (y_e - y_s > 0)
                    return 1; //向下滑动
                else if (y_e - y_s < 0)
                    return 2; //向上滑动
            }
            else if (x_e - x_s > 0)
                return 3; //向右滑动
            else if (x_e - x_s < 0)
                return 4; //向左滑动
        }
    }
}