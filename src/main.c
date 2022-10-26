#include <stdio.h>
#include "header.h"
#include "game.h"
#include "album.h"
#include "camera.h"
#include "music.h"
#include "video.h"

void main_process()
{
    while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            printf("x:%d--y:%d\n", P_I.x, P_I.y);
            if (P_I.x > 50 && P_I.x < 150 && P_I.y > 90 && P_I.y < 190) //相册按钮
            {
                // loadig();
                Album();
                show_mainUI();
            }

            if (P_I.x > 250 && P_I.x < 350 && P_I.y > 90 && P_I.y < 190) // mp3按钮
            {
                // loadig();
                Music();
                show_mainUI();
            }

            if (P_I.x > 450 && P_I.x < 550 && P_I.y > 90 && P_I.y < 190) // mp4按钮
            {
                // loadig();
                Video();
                show_mainUI();
            }

            if (P_I.x > 650 && P_I.x < 750 && P_I.y > 90 && P_I.y < 190) //游戏按钮
            {
                // Display_Pic("/windows_pic/exit.bmp", 0, 0, 800, 480); //显示主界面
                // printf("退出\n");
                // break;
                while (Game())
                    ; // Game返回1时代表重新开始游戏，则继续执行while循环，返回0时则退出游戏
                show_mainUI();
            }

            if (P_I.x > 50 && P_I.x < 150 && P_I.y > 290 && P_I.y < 390) //照相按钮
            {
                // loadig();
                Camera();
                show_mainUI();
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