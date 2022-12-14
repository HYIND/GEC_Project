#include "video.h"

#define FIFO_PATH "/tmp/video_Fifo"

P_Node cur_node; //当前正在观看的视频所在链表节点
P_Node head;

static int fd_fifo;
static bool stop_flag = false;
static bool play_flag = false;

static int bar_rate = 0;

static void show_bar() //显示进度条，bar_rate为百分比
{
    clear_lcd_screen(0x000000, 0, 350, 600, 30, p_lcd);                        // 进度条背景
    clear_lcd_screen(0x858585, 20, 360, 560, 10, p_lcd);                       // 进度条
    clear_lcd_screen(0xf38b00, 20, 360, (int)(bar_rate * 5.6), 10, p_lcd);     // 已播放的部分进度条绘橙色
    clear_lcd_screen(0x000000, 5 + (int)(bar_rate * 5.6), 357, 30, 16, p_lcd); // 滑块
}

static void show_UI()
{
}

static int send_cmd(int fd_fifo, const char *cmd)
{

    int ret_val = write(fd_fifo, cmd, strlen(cmd));
    if (-1 == ret_val)
    {
        perror("指令发送失败....\n");
        return -1;
    }

    printf("指令发送成功%d\n", ret_val);

    return 0;
}

void Init_Video()
{
    fd_fifo = -1;

    int ret_val;
    // 创建用于发送控制指令的管道文件
    if (access(FIFO_PATH, F_OK)) // 判断管道文件是否已存在
    {
        ret_val = mkfifo(FIFO_PATH, 0666);
        if (ret_val < 0)
        {
            perror("mkfifo error");
            return;
        }
        printf("管道文件创建成功\n");
    }

    //读取目录下的图片，生成双向链表
    head = New_Node(NULL);
    cur_node = head;

    char *photodir = "video";

    // 1.打开目录
    DIR *dp = opendir(photodir);

    // 2.读取目录项（包括文件名）
    struct dirent *temp;
    while (1)
    {
        temp = readdir(dp);
        if (temp == NULL) //读完，跳出
            break;

        //成功读取到
        // printf("%s\n", temp->d_name);

        if (strstr(temp->d_name, ".avi")) //判断是否为.avi文件
        {
            char *path = calloc(1, strlen(photodir) + strlen(temp->d_name) + 1);
            sprintf(path, "%s/%s", photodir, temp->d_name);
            P_Node new = New_Node(path);  //创建新节点
            List_Add_To(head, NULL, new); //插入新节点
        }
    }
    // 3.关闭目录
    closedir(dp);

    Display_List(head, true);

    bar_rate = 0;
}

void close_video()
{
    Release_List(head);
    head = cur_node = NULL;
    if (fd_fifo != -1)
    {
        send_cmd(fd_fifo, "quit\n");
        close(fd_fifo);
        fd_fifo = -1;
    }
    play_flag = false;
}

void switch_video(P_Node node, bool flag)
{
    if (fd_fifo != -1)
    {
        send_cmd(fd_fifo, "quit\n");
    }

    if (node == head)
    {
        if (flag)
            node = node->next;
        else
            node = node->prev;
    }

    if (node != head)
    {
        char *command = calloc(1, 256);
        sprintf(command, "mplayer %s -quiet -slave -geometry 0:0 -zoom -x 800 -y 400 -input file=%s &", node->Data, FIFO_PATH);
        // 直接使用 popen 来播放音视频
        FILE *fp = popen(command, "r");
        if (fp == NULL)
        {
            perror("popen error");
            return;
        }

        printf("%s\n", node->Data);
        // 打开用于发送指令的管道文件
        if (fd_fifo != -1)
            close(fd_fifo);
        fd_fifo = open(FIFO_PATH, O_RDWR);
        if (-1 == fd_fifo)
        {
            perror("open fifo error");
            return;
        }
        printf("视频正在播放....\n");
        play_flag = true;
    }
    show_videoUI(); //显示UI

    cur_node = node;
}

void Video()
{
    //初始化视频信息
    Init_Video();
    cur_node = head;

    show_videoUI();

    int tx = 0, ty = 0;

    stop_flag = false;
    while (!stop_flag)
    {
        int slide = get_ts(&tx, &ty); //获取触摸屏的坐标
        switch (slide)
        {
        case 0: // 非滑动
        {
            if (tx > 700 && ty > 380)
            {
                stop_flag = true; //退出
                break;
            }
            else
            {
                // 音量-
                if (tx < 240 && ty > 380)
                {
                    printf("%s\n", "voice-2");
                    send_cmd(fd_fifo, "volume -2\n");
                }
                else if (tx < 315 && ty > 380) //切换上一个视频
                {
                    printf("%s\n", "switch to last video");
                    switch_video(cur_node->prev, false);
                }
                // 播放
                else if (tx < 400 && ty > 380)
                {
                    printf("%s\n", "play");
                    if (play_flag == false)
                    {
                        send_cmd(fd_fifo, "pause\n");
                        play_flag = !play_flag;
                    }
                }
                // 停止
                else if (tx < 480 && ty > 380)
                {
                    printf("%s\n", "pause");
                    if (play_flag == true)
                    {
                        send_cmd(fd_fifo, "pause\n");
                        play_flag = !play_flag;
                    }
                }
                else if (tx < 560 && ty > 380) //切换下一个视频
                {
                    printf("%s\n", "switch to next video");
                    switch_video(cur_node->next, true);
                }
                // 音量+
                else if (tx < 640 && ty > 380)
                {
                    printf("%s\n", "voice+2");
                    send_cmd(fd_fifo, "volume +2\n");
                }
            }
        }
        break;
        case 1: //下滑
        {
            printf("%s\n", "voice-3");
            send_cmd(fd_fifo, "volume -3\n");
        }
        break;
        case 2: //上滑
        {
            printf("%s\n", "voice+3");
            send_cmd(fd_fifo, "volume +3\n");
        }
        break;
        case 3: //右滑
        {
            printf("%s\n", "seek +10");
            send_cmd(fd_fifo, "seek +10\n");
        }
        break;
        case 4: //左滑
        {
            printf("%s\n", "seek -10");
            send_cmd(fd_fifo, "seek -10\n");
        }
        break;
        }
    }

    close_video();
}
