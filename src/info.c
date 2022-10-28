#include "info.h"

void info()
{
    //打开字体
    font *points_font = fontLoad("./softbrush.ttf");
    //字体大小的设置
    fontSetSize(points_font, 24);

    bitmap *bm = createBitmapWithInit(500, 300, 4, getColor(0, 0, 0, 0));

    char info[] = "制作人：\n广大\n 计科194黄艺宣 1906100110 \n 计科194李龙威 1906100111 \n 计科194吴文海 1906100113";

    fontPrint(points_font, bm, 0, 0, info, getColor(0, 255, 255, 255), 500);

    //把字体框输出到LCD屏幕上
    show_font_to_lcd(p_lcd, 150, 90, bm);

    //关闭画板
    destroyBitmap(bm);

    //关闭字体
    fontUnload(points_font);

    while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
            break;
    }
}