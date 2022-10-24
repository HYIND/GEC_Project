#include "game.h"
#include "show_bmp.h"
#include "font.h"
#include <pthread.h>
#include <memory.h>

Ball ball;
Rect rect;
bool stop_flag = false;

font *points_font;
char points_buf[6] = "00000";

void draw_points()
{
    //创建一个画板（点阵图）
    bitmap *bm = createBitmapWithInit(140, 100, 4, getColor(0, 255, 255, 255));

    //将字体写到点阵图上
    fontPrint(points_font, bm, 10, 30, points_buf, getColor(0, 100, 100, 100), 140);

    //把字体框输出到LCD屏幕上
    show_font_to_lcd(p_lcd, 655, 80, bm);

    //关闭画板
    destroyBitmap(bm);
}

void draw_round()
{
}

void draw_rect()
{
}

void Init_Game()
{
    ball.x = 200;
    ball.y = 100;
    ball.radius = 10;
    ball.x_speed = 7;
    ball.y_speed = 5;

    rect.x = 400;
    rect.y = 420;
    rect.width = 100;
    rect.height = 25;
    rect.speed = 5;

    stop_flag = false;

    for (int i = 0; i < 800; i++)
    {
        for (int j = 0; j < 480; j++)
        {
            *(p_lcd + i + j * 800) = 0x000000;
        }
    }

    //打开字体
    points_font = fontLoad("./simkai.ttf");

    //字体大小的设置
    fontSetSize(points_font, 50);

    bitmap *bm = createBitmapWithInit(140, 60, 4, getColor(0, 0, 0, 0));
    char buf1[] = "得分";
    fontPrint(points_font, bm, 10, 0, buf1, getColor(0, 255, 255, 255), 140);
    show_font_to_lcd(p_lcd, 655, 20, bm);
    destroyBitmap(bm);

    bm = createBitmapWithInit(120, 70, 4, getColor(0, 0, 0, 0));
    char buf2[] = "退出";
    fontPrint(points_font, bm, 20, 0, buf2, getColor(0, 255, 255, 255), 120);
    show_font_to_lcd(p_lcd, 660, 380, bm);
    destroyBitmap(bm);

    memset(points_buf, '0', 5);
    draw_points();
}

void add_points()
{
    for (int i = 4; i >= 0; i--)
    {
        if (points_buf[i] < '9')
        {
            points_buf[i]++;
            return;
        }
        else
        {
            points_buf[i] = '0';
        }
    }
}

bool collision()
{
    if (ball.y + ball.radius > rect.y - rect.height / 2 && ball.y + ball.radius < rect.y + rect.height / 2 && ball.x > rect.x - rect.width / 2 && ball.x < rect.x + rect.width / 2)
    {
        ball.y = rect.y - rect.height / 2 - ball.radius;
        ball.y_speed += 2;
        ball.y_speed = (-ball.y_speed);

        add_points();
        draw_points();

        return true;
    }
    return false;
}

void move_rect()
{
    int pointX = P_I.x;
    if (P_I.x < rect.x)
    {
        if (pointX < rect.width / 2)
        {
            pointX = rect.width / 2;
        }

        if (rect.x > rect.width / 2)
        {
            if (rect.x - pointX < rect.speed)
                rect.x = pointX;
            else
                rect.x -= rect.speed;
        }
    }
    else if (pointX > rect.x)
    {
        if (pointX > 650 - rect.width / 2)
        {
            pointX = 650 - rect.width / 2;
        }
        if (pointX - rect.x < rect.speed)
            rect.x = pointX;
        else
            rect.x += rect.speed;
    }
}

void move_ball()
{
    ball.x += ball.x_speed;
    ball.y += ball.y_speed;

    if (collision())
    {
        ball.y_speed += 2;
    }

    if (ball.x - ball.radius < 0)
    {
        ball.x = ball.radius;
        ball.x_speed = (-ball.x_speed);
    }
    if (ball.x + ball.radius > 650)
    {
        ball.x = 650 - ball.radius;
        ball.x_speed = (-ball.x_speed);
    }
    if (ball.y - ball.radius < 0)
    {
        ball.y = ball.radius;
        ball.y_speed = (-ball.y_speed);
    }
    if (ball.y + ball.radius > 480)
    {
        // ball.y = 400 - ball.radius;
        // ball.y_speed = (-ball.y_speed);
        stop_flag = true;
    }
}

void draw()
{
    for (int i = 0; i < 650; i++)
    {
        for (int j = 0; j < 480; j++)
        {
            if ((i - ball.x) * (i - ball.x) + (j - ball.y) * (j - ball.y) < ball.radius * ball.radius)
            {
                *(p_lcd + i + j * 800) = 0xFF0000;
            }
            else if (i > rect.x - rect.width / 2 && i < rect.x + rect.width / 2 && j > rect.y - rect.height / 2 && j < rect.y + rect.height / 2)
            {
                *(p_lcd + i + j * 800) = 0x0000FF;
            }
            else
            {
                *(p_lcd + i + j * 800) = 0xFFFFFF;
            }
        }
    }
}

void control()
{
    bool isslide = false;
    while (1)
    {
        if (stop_flag)
            break;
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            printf("x:%d--y:%d\n", P_I.x, P_I.y);
            if (P_I.x > 660 && P_I.x < 780 && P_I.y > 380 && P_I.y < 450)
            {
                stop_flag = true;
                break;
            }
        }
        if (!isslide)
        {
            if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 1) //按下去
            {
                if (P_I.x > rect.x - rect.width / 2 - 20 && P_I.x < rect.x + rect.width / 2 + 20 && P_I.y > rect.y - rect.height / 2 - 20)
                {
                    isslide = true;
                    continue;
                }
            }
        }
        else
        {
            if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
            {
                isslide = false;
            }

            move_rect();
        }
    }
}

void Game()
{
    Init_Game();

    pthread_t control_thread;
    pthread_create(&control_thread, NULL, &control, NULL);
    while (1)
    {
        if (stop_flag)
            break;
        draw();
        // draw_round(&ball);
        // draw_rect(&rect);
        move_ball();
    }
    pthread_join(control_thread, NULL);

    bitmap *bm = createBitmapWithInit(250, 60, 4, getColor(0, 255, 255, 255));
    char buf1[] = "Game Over!";
    fontPrint(points_font, bm, 0, 0, buf1, getColor(0, 100, 100, 100), 250);
    show_font_to_lcd(p_lcd, 240, 200, bm);
    destroyBitmap(bm);

    bm = createBitmapWithInit(250, 60, 4, getColor(0, 255, 255, 255));
    char buf2[] = "点此处离开";
    fontPrint(points_font, bm, 0, 0, buf2, getColor(0, 100, 100, 100), 250);
    show_font_to_lcd(p_lcd, 230, 300, bm);
    destroyBitmap(bm);

    //关闭字体
    fontUnload(points_font);

    while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            if (P_I.x > 240 && P_I.x < 490 && P_I.y > 80 && P_I.y < 300)
            {
                break;
            }
        }
    }

    printf("exit game!\n");
}