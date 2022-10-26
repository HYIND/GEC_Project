#include "music.h"

#define FIFO_PATH "/tmp/music_Fifo"

P_Node cur_node; //当前正在播放的音频所在链表节点
P_Node head;

int fd_fifo;
bool stop_flag;

static void show_UI()
{
    show_location_bmp("windows_pic/musicUI.bmp", 0, 360, 600, 120, p_lcd);
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

void Init_Music()
{
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

    //读取目录下的音频，生成双向链表
    head = New_Node(NULL);
    cur_node = head;

    char *photodir = "music";

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

        if (strstr(temp->d_name, ".mp3")) //判断是否为.mp4/.avi文件
        {
            char *path = calloc(1, strlen(photodir) + strlen(temp->d_name) + 1);
            sprintf(path, "%s/%s", photodir, temp->d_name);
            P_Node new = New_Node(path);  //创建新节点
            List_Add_To(head, NULL, new); //插入新节点
        }
    }
    // 3.关闭目录
    closedir(dp);

    // Display_List(head, true);
}

void close_music()
{
    Release_List(head);
    head = cur_node = NULL;
    if (fd_fifo != -1)
        close(fd_fifo);
    stop_flag = false;
}

void switch_music(P_Node node)
{
    if (fd_fifo != -1)
    {
        send_cmd(fd_fifo, "quit\n");
    }

    if (node != head)
    {
        char *command = calloc(1, 256);
        sprintf(command, "mplayer %s -input file=%s &", node->Data, FIFO_PATH);
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
        stop_flag = false;
    }
    show_UI(); //显示UI

    cur_node = node;
}

void Music()
{
    //初始化视频信息
    Init_Music();

    switch_music(head);

    int tx = 0, ty = 0;

    bool stop = false;
    while (!stop)
    {
        int slide = get_ts(&tx, &ty); //获取触摸屏的坐标
        switch (slide)
        {
        case 0: // 非滑动
        {
            if (tx > 700 && ty < 100)
            {
                stop = true; //退出
                break;
            }
            else
            {
                // 停止/播放
                if (300 < tx & tx < 500 && ty > 300)
                {
                    send_cmd(fd_fifo, "pause\n");
                    stop_flag = !stop_flag;
                }

                if (tx < 100 && tx > 0) //切换下一个视频
                {
                    switch_music(cur_node->prev);
                }
                else if (tx > 700 && tx < 800) //切换上一个视频
                {
                    switch_music(cur_node->next);
                }
            }
        }
        break;
        case 1:
            break;
        case 2:
            break;
        case 3: //左滑
        {
            // switch_music(cur_node->prev);
        }
        break;

        case 4: //右滑
        {
            // switch_music(cur_node->next);
        }
        break;
        }
    }

    close_music();
}
