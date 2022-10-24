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