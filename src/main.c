#include <stdio.h>
#include "header.h"
#include "game.h"

int main()
{
    my_Init();

    show_UI();
    while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            printf("x:%d--y:%d\n", P_I.x, P_I.y);
            if (P_I.x > 20 && P_I.x < 220 && P_I.y > 300 && P_I.y < 440) //视频按钮
            {
                // loadig();
                // Video();
            }

            if (P_I.x > 310 && P_I.x < 480 && P_I.y > 300 && P_I.y < 440) //音乐按钮
            {
                // loadig();
                // Music();
            }

            if (P_I.x > 580 && P_I.x < 770 && P_I.y > 300 && P_I.y < 440) //游戏按钮
            {
                // loadig();
                Game();
                show_UI();
            }

            if (P_I.x > 340 && P_I.x < 440 && P_I.y > 180 && P_I.y < 280) //退出按钮
            {
                // Display_Pic("/windows_pic/exit.bmp", 0, 0, 800, 480); //显示主界面
                // printf("退出\n");
                // break;
            }
        }
    }

    my_close();
    return 0;
}