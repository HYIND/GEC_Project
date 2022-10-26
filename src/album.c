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

static void show_UI()
{
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

void switch_photo(P_Node node)
{
    clear_lcd_screen(0XFFFFFF, 0, 0, 800, 480, p_lcd);
    // show_1152000bmp("windows_pic/albumbkg.bmp", p_lcd); //开发板下，可执行文件目录下存放的背景图（可以使用绝对路径！）
    if (node != head)
    {
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
    show_UI(); //显示UI

    cur_node = node;
}

void Album()
{
    //初始化相册信息
    Init_Album();

    cur_node = head;

    int tx = 0, ty = 0;

    bool stop = false;
    while (!stop)
    {
        int slide = get_ts(&tx, &ty); //获取触摸屏的坐标
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
                    switch_photo(cur_node->prev);
                }
                else if (tx > 700 && tx < 800) //右翻
                {
                    switch_photo(cur_node->next);
                }
            }
        }
        break;

        case 3: //左滑
        {
            switch_photo(cur_node->prev);
        }
        break;

        case 4: //右滑
        {
            switch_photo(cur_node->next);
        }
        break;
        }
    }
}