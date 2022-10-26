#include "UI.h"
#include "fd.h"

void show_mainUI()
{
    show_1152000bmp("windows_pic/main.bmp", p_lcd);
    show_location_bmp("windows_pic/photo.bmp",50,90,100,100,p_lcd);
    show_location_bmp("windows_pic/mp3.bmp",250,90,100,100,p_lcd);
    show_location_bmp("windows_pic/mp4.bmp",450,90,100,100,p_lcd);
    show_location_bmp("windows_pic/game.bmp",650,90,100,100,p_lcd);
    show_location_bmp("windows_pic/camera.bmp",50,290,100,100,p_lcd);
}

void show_musicUI()
{
    show_1152000bmp("windows_pic/main.bmp",p_lcd);

}


void show_gameoverUI()
{
    show_location_bmp("windows_pic/gameover.bmp",280,200,240,80,p_lcd);
    show_location_bmp("windows_pic/restart.bmp",240,320,100,43,p_lcd);
    show_location_bmp("windows_pic/exit.bmp",380,320,100,43,p_lcd);
}