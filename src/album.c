#include "album.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "deque.h"
#include "show_bmp.h"

P_Node cur_node; //当前正在访问的图片所在链表节点
P_Node head;

bool slide_flag = false;

static void show_Bkg()
{
    show_1152000bmp("windows_pic/mainbkg.bmp", p_lcd); //背景
}

void Init_Album()
{
    //读取目录下的图片，生成双向链表
    head = New_Node("");
    cur_node = head;

    char *photodir = "photo";

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

        if (strstr(temp->d_name, ".bmp") || strstr(temp->d_name, ".jpg")) //判断是否为.bmp/.jpg文件
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

void switch_photo(P_Node node, bool flag) // flag 向前/向后标志
{
    if (node == head)
    {
        if (flag)
            node = node->next;
        else
            node = node->prev;
    }

    if (node != head)
    {
        printf("%s\n", node->Data);

        if (strstr(node->Data, ".bmp"))
        {
            if (show_1152000bmp(node->Data, p_lcd) != 0) //显示BMP图片
                show_Bkg();
        }
        if (strstr(node->Data, ".jpg"))
        {
            if (lcd_draw_jpg(0, 0, node->Data, NULL, NULL, 0) != 0) //显示JPG图片

                show_Bkg();
        }
    }

    show_albumUI(); //显示UI

    cur_node = node;
}

void switch_slide_photo(P_Node node)
{
    if (node == head)
    {
        node = node->next;
    }

    if (node != head)
    {
        srand((unsigned)time(NULL));
        int flag = rand() % 5;
        switch (flag)
        {
        case 0:
            pic_circular_spread(node->Data, p_lcd);
            break;
        case 1:
            pic_down(node->Data, p_lcd);
            break;
        case 2:
            pic_transverse_blinds(node->Data, p_lcd);
            break;
        case 3:
            pic_mid_spread(node->Data, p_lcd);
            break;
        case 4:
            pic_oblique_block(node->Data, p_lcd);
            break;
        default:
            show_1152000bmp(node->Data, p_lcd);
            break;
        }

        printf("%s\n", node->Data);

        if (strstr(node->Data, ".bmp"))
        {
            show_1152000bmp(node->Data, p_lcd); //显示BMP图片
        }
        if (strstr(node->Data, ".jpg"))
        {
            lcd_draw_jpg(0, 0, node->Data, NULL, NULL, 0); //显示JPG图片
        }
    }
}

void slide_photo(P_Node node)
{
    P_Node cur_slide = node;
    do
    {
        switch_slide_photo(cur_slide);
        cur_slide = cur_slide->next;
        sleep(2);
    } while (slide_flag && cur_slide != node);
    switch_photo(node, true);
    printf("slide exit!\n");
}

void delete_photo(P_Node node)
{
    if (node == head)
        return;

    switch_photo(node->next, true);
    // char command[256] = {0};
    // sprintf(command, "rm %s", node->Data);
    // printf("%s\n", command);
    // system(command);

    if (remove(node->Data) == 0)
    {
        printf("Removed %s.", node->next);
    }
    else
    {
        perror("remove");
    }
    Delete_Node(node);
}

void Album()
{
    //初始化相册信息
    Init_Album();

    cur_node = head;
    show_Bkg();
    show_albumUI();

    pthread_t slide_thread;

    slide_flag = false;
    int tx = 0, ty = 0;

    bool stop = false;
    while (!stop)
    {
        int slide = get_ts(&tx, &ty); //获取触摸屏的坐标
        if (slide_flag)
        {
            slide_flag = false;
        }
        else
        {
            switch (slide)
            {
            case 0: // 非滑动
            case 1:
            case 2:
            {
                if (tx > 700 && ty < 100)
                {
                    stop = true; //退出
                    break;
                }
                else
                {
                    if (tx < 100 && tx > 0) //左翻
                    {
                        switch_photo(cur_node->prev, false);
                    }
                    else if (tx > 700 && tx < 800) //右翻
                    {
                        switch_photo(cur_node->next, true);
                    }
                    else if (tx > 330 && tx < 390 && ty > 420) //从该图片开始播放幻灯片
                    {
                        slide_flag = true;
                        pthread_create(&slide_thread, NULL, &slide_photo, cur_node); //幻灯片
                    }
                    else if (tx > 410 && tx < 470 && ty > 420) //删除图片
                    {
                        printf("delete\n");
                        delete_photo(cur_node);
                    }
                }
            }
            break;

            case 3: //左滑
            {
                switch_photo(cur_node->prev, false);
            }
            break;

            case 4: //右滑
            {
                switch_photo(cur_node->next, true);
            }
            break;
            }
        }
    }
}