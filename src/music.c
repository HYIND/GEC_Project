#include "music.h"

#define FIFO_PATH "/tmp/music_Fifo"

P_Node cur_node; //当前正在播放的音频所在链表节点
P_Node head;

P_Node cur_list_node_start;
P_Node cur_list_node_end;

static int fd_fifo;
static bool stop_flag = false; //程序停止的flag
static bool play_flag = false; //音频播放状态的标志位

static int bar_rate = 0;

P_Node page_index[100] = {NULL}; //第1、2、3…………页的索引首节点
int sum_index = 0;               //总共索引节点数量，即总页数
int cur_index = 0;               //当前所在页数

font *text_font;

static void show_bar() //显示进度条，bar_rate为百分比
{
    clear_lcd_screen(0x000000, 0, 350, 500, 30, p_lcd);                        // 进度条背景
    clear_lcd_screen(0x858585, 20, 360, 460, 10, p_lcd);                       // 进度条
    clear_lcd_screen(0xf38b00, 20, 360, (int)(bar_rate * 4.6), 10, p_lcd);     // 已播放的部分进度条绘橙色
    clear_lcd_screen(0x000000, 5 + (int)(bar_rate * 4.6), 357, 30, 16, p_lcd); // 滑块
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
    int mp3_count = 0;
    sum_index = 0;
    cur_index = 0;

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

            mp3_count++;
            if (mp3_count % 5 == 1) //取第1、6、11…………首歌曲作为每一页的索引节点
            {
                page_index[sum_index] = new;
                sum_index++;
            }
        }
    }

    // 3.关闭目录
    closedir(dp);

    // Display_List(head, true);
    bar_rate = 0;

    //打开字体
    text_font = fontLoad("./softbrush.ttf");
}

void close_music()
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

    //关闭字体
    fontUnload(text_font);
}

void switch_music(P_Node node, bool flag) // flag 向前/向后标志
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
        sprintf(command, "mplayer %s -quiet -slave -input file=%s &", node->Data, FIFO_PATH);
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

    cur_node = node;
}

static void bar() // 进度条线程
{
    while (!stop_flag)
    {
        if (play_flag) //音频正在播放则更新进度条信息
        {
            // 1.获取进度条百分比

            // 2.根据百分比绘进度条
            show_bar();
        }
        usleep(500000);
    }
}

void show_list_node(int count, P_Node node)
{
    //打开字体
    text_font = fontLoad("./softbrush.ttf");
    //字体大小的设置
    fontSetSize(text_font, 16);

    bitmap *bm = createBitmapWithInit(290, 70, 4, getColor(0, 0, 0, 0));

    fontPrint(text_font, bm, 10, 30, node->Data, getColor(0, 255, 255, 255), 290);

    //把字体框输出到LCD屏幕上
    show_font_to_lcd(p_lcd, 510, 70 + 70 * count, bm);

    //关闭画板
    destroyBitmap(bm);
}

/* void show_music_list()
{
    clear_lcd_screen(0x000000, 500, 70, 200, 350, p_lcd);
    cur_list_node_start = cur_list_node_end->next;
    if (cur_list_node_start == head)
        cur_list_node_start = cur_list_node_start->next;

    P_Node cur_node = cur_list_node_start;

    int count = 0;
    while (count < 5 && cur_node != head)
    {
        show_list_node(count, cur_node);
        cur_node = cur_node->next;
        count++;
    }
    cur_list_node_end = cur_node->prev;
} */

/* void list_play(int count)
{
    P_Node cur_node = cur_list_node_start;
    while (count)
    {
        cur_node = cur_node->next;
        if (cur_node == head)
            return;
        count--;
    }
    switch_music(cur_node, true);
} */

void show_music_list(int jump_index) // jump_index为跳转页数，从开始
{
    printf("cur_index:%d\n", jump_index);
    printf("sum_index:%d\n", sum_index);
    if (jump_index <= 0 && jump_index > sum_index || sum_index == 0)
        return;

    P_Node cur_node = page_index[jump_index - 1];

    clear_lcd_screen(0x000000, 500, 70, 200, 350, p_lcd);
    int count = 0;
    while (count < 5 && cur_node != head)
    {
        printf("cur_data:%s\n", cur_node->Data);
        show_list_node(count, cur_node);
        cur_node = cur_node->prev;
        count++;
    }

    cur_index = jump_index;
}

void switch_next_page()
{
    cur_index++;
    if (cur_index > sum_index)
    {
        cur_index = 1;
    }
    show_music_list(cur_index);
}

void switch_prev_page()
{
    cur_index--;
    if (cur_index < 1)
    {
        cur_index = sum_index;
    }
    show_music_list(cur_index);
}

void list_play(int count)
{
    P_Node cur_node = page_index[cur_index - 1];
    while (count)
    {
        cur_node = cur_node->prev;
        if (cur_node == head)
            return;
        count--;
    }
    switch_music(cur_node, true);
}

void Music()
{
    //初始化视频信息
    Init_Music();
    cur_node = head;
    cur_list_node_start = head->next;
    cur_list_node_end = head;
    show_musicUI();
    show_music_list(1); //显示第一页列表

    int tx = 0, ty = 0;

    // pthread_t bar_thread;
    // pthread_create(&bar_thread, NULL, &bar, NULL);

    stop_flag = false;
    while (!stop_flag)
    {
        int slide = get_ts(&tx, &ty); //获取触摸屏的坐标
        switch (slide)
        {
        case 0: // 非滑动
        {
            if (tx > 700 && ty < 100)
            {
                stop_flag = true; //退出
                break;
            }
            else
            {
                // 音量-
                if (tx < 90 && ty > 380)
                {
                    printf("%s\n", "voice--");
                    send_cmd(fd_fifo, "volume -2\n");
                }
                else if (tx < 165 && ty > 380) //切换上一首音乐
                {
                    printf("%s\n", "switch to last music");
                    switch_music(cur_node->prev, false);
                }
                // 播放
                else if (tx < 250 && ty > 380)
                {
                    printf("%s\n", "play");
                    if (play_flag == false)
                    {
                        send_cmd(fd_fifo, "pause\n");
                        play_flag = !play_flag;
                    }
                }
                // 停止
                else if (tx < 330 && ty > 380)
                {
                    printf("%s\n", "pause");
                    if (play_flag == true)
                    {
                        send_cmd(fd_fifo, "pause\n");
                        play_flag = !play_flag;
                    }
                }
                else if (tx < 410 && tx > 380) //切换下一首音乐
                {
                    printf("%s\n", "switch to next music");
                    switch_music(cur_node->next, true);
                }
                // 音量+
                else if (tx < 490 && ty > 380)
                {
                    printf("%s\n", "voice++");
                    send_cmd(fd_fifo, "volume +2\n");
                }
                else if (tx > 500 && ty < 140)
                {
                    list_play(0);
                }
                else if (tx > 500 && ty < 210)
                {
                    list_play(1);
                }
                else if (tx > 500 && ty < 280)
                {
                    list_play(2);
                }
                else if (tx > 500 && ty < 350)
                {
                    list_play(3);
                }
                else if (tx > 500 && ty < 420)
                {
                    list_play(4);
                }
                else if (tx > 500 && tx < 600 && ty > 420) //上一页
                {
                    printf("last page\n");
                    switch_prev_page();
                }
                else if (tx > 700 && ty > 420) //下一页
                {
                    printf("next page\n");
                    switch_next_page();
                }
            }
        }
        break;
        }
    }
    // pthread_join(bar_thread, NULL);

    close_music();
}
