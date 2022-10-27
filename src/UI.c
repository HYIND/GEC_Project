#include "UI.h"
#include "fd.h"

void show_mainUI()
{
    show_1152000bmp("windows_pic/mainbkg.bmp", p_lcd);
    show_location_bmp("windows_pic/photo.bmp", 50, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/mp3.bmp", 250, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/mp4.bmp", 450, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/game.bmp", 650, 90, 100, 100, p_lcd);
    show_location_bmp("windows_pic/camera.bmp", 50, 290, 100, 100, p_lcd);
}

void show_albumUI()
{
    show_location_bmp("windows_pic/right.bmp", 760, 205, 40, 70, p_lcd);
    show_location_bmp("windows_pic/left.bmp", 0, 205, 40, 70, p_lcd);
    show_location_bmp("windows_pic/slide.bmp", 330, 420, 52, 52, p_lcd);
    show_location_bmp("windows_pic/delete.bmp", 418, 420, 52, 52, p_lcd);
    show_location_bmp("windows_pic/exitphoto.bmp", 700, 0, 100, 100, p_lcd);
}

void show_cameraUI()
{

    clear_lcd_screen(0x000000, 0, 0, 800, 480, p_lcd);
    show_location_bmp("windows_pic/exitcamera.bmp", 675, 10, 100, 100, p_lcd);
    show_location_bmp("windows_pic/takephoto.bmp", 675, 190, 100, 100, p_lcd);
    show_location_bmp("windows_pic/photo2.bmp", 675, 370, 100, 100, p_lcd);
}

void show_musicUI()
{
    show_1152000bmp("windows_pic/mainbkg.bmp", p_lcd);                     // 音乐界面背景
    show_location_bmp("windows_pic/musicUI.bmp", 0, 380, 500, 100, p_lcd); // 音乐界面控制台
    show_location_bmp("windows_pic/bigmusic.bmp", 125, 50, 250, 250, p_lcd);
}

void show_videoUI()
{
    clear_lcd_screen(0x000000, 0, 0, 800, 480, p_lcd);
    show_location_bmp("windows_pic/musicUI.bmp", 150, 380, 500, 100, p_lcd); // 视频界面控制台
}

void show_gameoverUI()
{
    show_location_bmp("windows_pic/restartgame.bmp", 240, 220, 100, 43, p_lcd);
    show_location_bmp("windows_pic/gameover.bmp", 205, 200, 240, 80, p_lcd);
    show_location_bmp("windows_pic/exit.bmp", 380, 330, 100, 43, p_lcd);
}