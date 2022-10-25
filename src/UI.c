#include "UI.h"
#include "fd.h"



void show_mainUI()
{
    show_1152000bmp("windows_pic/main.bmp", p_lcd);
    show_location_bmp("windows_pic/camera.bmp",50,190,100,100,p_lcd);
    show_location_bmp("windows_pic/mp3.bmp",250,190,100,100,p_lcd);
    show_location_bmp("windows_pic/mp4.bmp",450,190,100,100,p_lcd);
    show_location_bmp("windows_pic/game.bmp",650,190,100,100,p_lcd);
}