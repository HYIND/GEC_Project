#include "camera.h"

//设置摄像头启动变量
int video_show_flag = 0;
//设置抓拍变量
int take_photo_flag = 0;

void *real_time_video()
{
    printf("%c", "real_time_video");
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
            char path[256] = {0};
            time_t now;
            time(&now);
            sprintf(path, "%s%d.jpg", "photo/camera_photo_", now);
            //创建一个新的文件
            int fd = open(path, O_RDWR | O_CREAT, 0777);
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

// 相机相册
void camera_album()
{
}

void Camera()
{
    //创建一条线程实时监控
    pthread_t pid;
    video_show_flag = 1;
    pthread_create(&pid, NULL, &real_time_video, NULL);

    show_cameraUI();

    int x, y;
    while (1)
    {
        get_ts(&x, &y);
        if (x > 670 && y < 150)
        {
            video_show_flag = 0;
            break; //退出
        }
        else if (x > 670 && y > 165 && y < 315)
        {
            //抓拍
            take_photo_flag = 1;
            printf("拍照\n");
        }
        else if (x > 670 && y > 330)
        {
            // 相册
            video_show_flag = 0;
            pthread_join(pid, NULL);
            camera_album();
            show_cameraUI();
            video_show_flag = 1;
            pthread_create(&pid, NULL, &real_time_video, NULL);
        }
    }
    printf("关闭摄像头\n");
    video_show_flag = 0;
    pthread_join(pid, NULL);
}