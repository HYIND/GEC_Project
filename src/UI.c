#include "UI.h"
#include "fd.h"

void show_mainUI()
{
    show_1152000bmp("windows_pic/main.bmp", p_lcd);
    show_location_bmp("windows_pic/photo.bmp", 50, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/mp3.bmp", 250, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/mp4.bmp", 450, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/game.bmp", 650, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/camera.bmp", 50, 290, 100, 100, p_lcd);
}

void show_musicUI()
{
    show_1152000bmp("windows_pic/main.bmp", p_lcd);
}

void show_gameoverUI()
{
    show_location_bmp("windows_pic/gameover.bmp", 280, 200, 240, 80, p_lcd);
    show_location_bmp("windows_pic/restartgame.bmp", 240, 320, 100, 43, p_lcd);
    show_location_bmp("windows_pic/gameover.bmp", 205, 200, 240, 80, p_lcd);
    show_location_bmp("windows_pic/restart.bmp", 240, 320, 100, 43, p_lcd);
    show_location_bmp("windows_pic/exit.bmp", 380, 320, 100, 43, p_lcd);
}

void show_cameraUI()
{

     for (int i = 700; i < 800; i++)
    {
        for (int j = 0; j < 480; j++)
        {
            *(p_lcd + i + j * 800) = 0x000000;
        }
    }
     show_location_bmp("windows_pic/takephoto.bmp",700,190, 100, 100, p_lcd);
     show_location_bmp("windows_pic/exit.bmp", 700, 0, 100 , 100, p_lcd);
}