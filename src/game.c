#include "game.h"
#include "show_bmp.h"
#include "font.h"
#include <pthread.h>
#include <memory.h>

#define RADUIS 18
#define HALF_RECT_WIDTH 60
#define HALF_RECT_HEIGHT 12

Ball ball;
Rect rect;
bool stop_flag = false;

font *points_font;
char points_buf[6] = "00000";
int add_count = 1;

bool AD()
{
    return false;
}

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

void draw_ball()
{
    static int lastdraw_ball_x = 300;
    static int lastdraw_ball_y = 200;

    int left = lastdraw_ball_x - ball.radius;
    int right = lastdraw_ball_x + ball.radius;
    int top = lastdraw_ball_y - ball.radius;
    int bottom = lastdraw_ball_y + ball.radius;
    // 刷新原小球背景
    for (int i = left; i < right; i++)
    {
        for (int j = top; j < bottom; j++)
        {
            *(p_lcd + i + j * 800) = 0xFFFFFF;
        }
    }

    // 画新小球
    lastdraw_ball_x = ball.x;
    lastdraw_ball_y = ball.y;
    left = lastdraw_ball_x - ball.radius;
    right = lastdraw_ball_x + ball.radius;
    top = lastdraw_ball_y - ball.radius;
    bottom = lastdraw_ball_y + ball.radius;
    // for (int i = left; i < right; i++)
    // {
    //     for (int j = top; j < bottom; j++)
    //     {
    //         if ((i - lastdraw_ball_x) * (i - lastdraw_ball_x) + (j - lastdraw_ball_y) * (j - lastdraw_ball_y) < ball.radius * ball.radius)
    //         {
    //             *(p_lcd + i + j * 800) = 0xFF0000;
    //         }
    //     }
    // }
    show_location_bmp("game_pic/basketball.bmp", left, top, ball.radius * 2, ball.radius * 2, p_lcd);
}

void draw_rect()
{
    static int lastdraw_rect_x = 400;
    static int lastdraw_rect_y = 420;

    int left = lastdraw_rect_x - rect.width / 2;
    int right = lastdraw_rect_x + rect.width / 2;
    int top = lastdraw_rect_y - rect.height / 2;
    int bottom = lastdraw_rect_y + rect.height / 2;

    // 刷新原木板背景
    for (int i = left; i < right; i++)
    {
        for (int j = top; j < bottom; j++)
        {
            *(p_lcd + i + j * 800) = 0xFFFFFF;
        }
    }

    // 画新木板
    lastdraw_rect_x = rect.x;
    lastdraw_rect_y = rect.y;
    left = lastdraw_rect_x - rect.width / 2;
    right = lastdraw_rect_x + rect.width / 2;
    top = lastdraw_rect_y - rect.height / 2;
    bottom = lastdraw_rect_y + rect.height / 2;
    for (int i = left; i < right; i++)
    {
        for (int j = top; j < bottom; j++)
        {
            *(p_lcd + i + j * 800) = 0x0000FF;
        }
    }

    // show_location_bmp("basketball_kun.bmp", left, top, rect.width, rect.height, p_lcd);
}

void draw()
{
    draw_ball();
    draw_rect();
}

void add_points(int count)
{
    while (count > 0)
    {
        for (int i = 4; i >= 0; i--)
        {
            if (points_buf[i] < '9')
            {
                points_buf[i]++;
                break;
            }
            else
            {
                points_buf[i] = '0';
            }
        }
        count--;
    }
}

bool collision()
{
    if (ball.y + ball.radius > rect.y - rect.height / 2 && ball.y + ball.radius < rect.y + rect.height / 2 && ball.x > rect.x - rect.width / 2 && ball.x < rect.x + rect.width / 2)
    {
        ball.y = rect.y - rect.height / 2 - ball.radius;
        ball.y_speed += 2;
        ball.y_speed = (-ball.y_speed);

        add_points(add_count);
        add_count++;
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
        if (AD())
        {
            ball.y = 400 - ball.radius;
            ball.y_speed = (-ball.y_speed);
        }
        else
        {
            stop_flag = true;
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

void show_begin()
{
    // 绘制游戏区白色背景
    for (int i = 0; i < 650; i++)
    {
        for (int j = 0; j < 480; j++)
        {
            *(p_lcd + i + j * 800) = 0xFFFFFF;
        }
    }

    // 绘制功能区黑色背景
    for (int i = 650; i < 800; i++)
    {
        for (int j = 0; j < 480; j++)
        {
            *(p_lcd + i + j * 800) = 0x000000;
        }
    }

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

void show_end()
{
    bitmap *bm = createBitmapWithInit(250, 60, 4, getColor(0, 255, 255, 255));
    char buf1[] = "Game Over!";
    fontPrint(points_font, bm, 0, 0, buf1, getColor(0, 100, 100, 100), 250);
    show_font_to_lcd(p_lcd, 240, 200, bm);
    destroyBitmap(bm);

    bm = createBitmapWithInit(500, 60, 4, getColor(0, 255, 255, 255));
    char buf2[] = "重新开始   退出游戏";
    fontPrint(points_font, bm, 0, 0, buf2, getColor(0, 100, 100, 100), 500);
    show_font_to_lcd(p_lcd, 100, 300, bm);
    destroyBitmap(bm);
}

void Init_Game()
{
    ball.x = 200;
    ball.y = 100;
    ball.radius = RADUIS;
    ball.x_speed = 5;
    ball.y_speed = 3;

    rect.x = 400;
    rect.y = 420;
    rect.width = HALF_RECT_WIDTH * 2;
    rect.height = HALF_RECT_HEIGHT * 2;
    rect.speed = 5;

    stop_flag = false;

    add_count = 1;

    //打开字体
    points_font = fontLoad("./simkai.ttf");
    //字体大小的设置
    fontSetSize(points_font, 50);
}

bool Game()
{
    Init_Game();

    //绘制初始界面
    show_begin();

    //控制线程
    pthread_t control_thread;
    pthread_create(&control_thread, NULL, &control, NULL);
    while (1)
    {
        if (stop_flag)
            break;
        draw();

        usleep(30000);
        // draw_round(&ball);
        // draw_rect(&rect);
        move_ball();
    }
    pthread_join(control_thread, NULL);

    //绘制介绍界面
    show_end();
    //关闭字体
    fontUnload(points_font);

    while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            // 重新开始
            if (P_I.x > 90 && P_I.x < 250 && P_I.y > 280 && P_I.y < 380)
            {
                printf("restart game!\n");
                return true;
            }
            // 退出游戏
            else if (P_I.x > 290 && P_I.x < 470 && P_I.y > 280 && P_I.y < 380)
            {
                printf("exit game!\n");
                return false;
            }
        }
    }
}