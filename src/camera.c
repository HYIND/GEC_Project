#include "camera.h"
#include <stdio.h>
#include "lcd.h"
#include "yuyv.h"
#include <pthread.h>
#include "show_bmp.h"

//设置摄像头启动变量
int video_show_flag = 0;
//设置抓拍变量
int take_photo_flag = 0;

void *real_time_video()
{
    struct jpg_data video_buf;

    //初始化摄像头设备
    linux_v4l2_yuyv_init("/dev/video7");

    //启动摄像头
    linux_v4l2_start_yuyv_capturing();

    while (1)
    {

        //获取摄像头数据
        linux_v4l2_get_yuyv_data(&video_buf);

        //显示摄像头图像
        lcd_draw_jpg(0, 0, NULL, video_buf.jpg_data, video_buf.jpg_size, 0);

        if (take_photo_flag == 1) //进行抓拍
        {
            //创建一个新的文件
            int fd = open("photo/photo.jpg", O_RDWR | O_CREAT, 0777);
            if (fd == -1)
            {
                perror("拍照失败");
                continue;
            }
            //将图片数据写入文件中
            write(fd, video_buf.jpg_data, video_buf.jpg_size);
            //关闭文件保存
            close(fd);
            //清空抓拍的标志位
            take_photo_flag = 0;
        }
        if (video_show_flag == 0)
            break;
    }

    //停止摄像头
    linux_v4l2_yuyv_quit();
}

void camera()
{

    //打开LCD屏幕
    lcd_open("/dev/fb0");

    //创建一条线程实时监控
    pthread_t pid;

    int x, y;
    while (1)
    {
        get_ts(&x, &y);
        if (x < 100 && y < 80)
        {
            break; //退出
        }
        else if (x > 700 && y < 80)
        {
            take_photo_flag = 1; //抓拍
            printf("抓拍\n");
        }
        else if (x > 700 && y > 400)
        {
            lcd_draw_jpg(0, 0, "photo/photo.jpg", NULL, NULL, 0);
        }
        else if (x < 400)
        {
            printf("打开摄像头\n");
            pthread_create(&pid, NULL, real_time_video, NULL);
            video_show_flag = 1;
        }
        else if (x > 400)
        {
            printf("关闭摄像头\n");
            video_show_flag = 0;
        }
    }

    //关闭LCD屏幕
    close_lcd();

    return 0;
}