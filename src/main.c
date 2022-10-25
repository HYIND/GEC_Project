#include <stdio.h>
#include "header.h"
#include "game.h"
#include "album.h"

void main_process()
{
    while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            printf("x:%d--y:%d\n", P_I.x, P_I.y);
            if (P_I.x > 20 && P_I.x < 220 && P_I.y > 300 && P_I.y < 440) //相册按钮
            {
                // loadig();
                Album();
                // show_mainUI();
            }

            if (P_I.x > 310 && P_I.x < 480 && P_I.y > 300 && P_I.y < 440) //多媒体按钮
            {
                // loadig();
                // Music();
                // show_mainUI();
            }

            if (P_I.x > 580 && P_I.x < 770 && P_I.y > 300 && P_I.y < 440) //游戏按钮
            {
                // loadig();
                while(Game());      //Game返回1时代表重新开始游戏，则继续执行while循环，返回0时则退出游戏
                show_mainUI();
            }

            if (P_I.x > 340 && P_I.x < 440 && P_I.y > 180 && P_I.y < 280) //退出按钮
            {
                // Display_Pic("/windows_pic/exit.bmp", 0, 0, 800, 480); //显示主界面
                // printf("退出\n");
                // break;
            }
        }
    }
}

int main()
{
    my_Init();

    show_mainUI();
    main_process();

    my_close();
    return 0;
}