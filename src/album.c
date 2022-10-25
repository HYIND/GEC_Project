#include "album.h"
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>


P_Node cur_node;    //当前正在访问的图片所在链表节点
P_Node head;

P_Node New_Node(data_type path)
{

    P_Node new = calloc(1, sizeof(Node));

    // 初始化节点内容
    new->Data = num;
    new->next = new->prev = new;

    return new;
}

int List_Add_To(P_Node head, data_type num, P_Node new)
{
    if (head == NULL)
    {
        printf("链表头异常");
        return -1;
    }

    if (new == NULL)
    {
        //创建一个新节点
        new = New_Node(num);
    }

    //插入数据
    new->next = head->next;
    new->prev = head;

    head->next->prev = new;
    head->next = new;

    return 0;
}

int Display_List(P_Node head, bool flag)
{
    if (head == NULL || head->next == head)
    {
        printf("链表异常，链表为空\n");
        return -1;
    }

    if (flag) //为真则往后遍历
    {
        for (P_Node tmp = head->next; tmp != head; tmp = tmp->next)
        {
            printf("%d\n", tmp->Data);
        }
    }
    else //为假则往后遍历
    {
        for (P_Node tmp = head->next; tmp != head; tmp = tmp->prev)
        {
            printf("%d\n", tmp->Data);
        }
    }

    return 0;
}

//查找
/* P_Node Find_List(P_Node head, data_type num)
{
    if (head == NULL || head->next == head)
    {
        printf("链表异常,或链表为空 ....\n");
        return NULL;
    }

    P_Node tmp = head->next;
    for (; tmp != head; tmp = tmp->next)
    {
        if (tmp->Data == num)
        {
            return tmp;
        }
    }

    return NULL;
} */

//删除节点
/* P_Node Del_List(P_Node head, data_type num, P_Node del)
{
    if (head == NULL)
    {
        printf("链表异常 ....\n");
        return NULL;
    }

    P_Node del = Find_List(head, num);
    if (del == NULL)
    {
        printf("查找失败，没有找到需要剔除的数据....\n");
        return NULL;
    }

    // 剔除节点
    del->prev->next = del->next;
    del->next->prev = del->prev;

    // 把该节点重新初始化 \  
    del->next = del->prev = del;

    return del;
} */

void Init_Album(data_type num)
{
    //读取目录下的图片，生成双向链表
	
	// 1.打开目录
	DIR *dp = opendir("photo");
	
	// 2.读取目录项（包括文件名）
	struct dirent *temp;
	while(1)
	{
		temp = readdir(dp);
		if(temp == NULL)	//读完，跳出
			break;

            //成功读取到
	    printf("%s\n", temp->d_name);
		
        if(strstr(temp->d_name,".bmp")) //判断是否为.bmp文件
			{
				struct P_Node *new = New_Node(temp->d_name); //创建新节点
				List_Add_To(head,num[1],new);  //插入新节点
	        }
    }
	// 3.关闭目录
	closedir(dp);


}

void Album(data_type num)
{
    //初始化相册信息
    Init_Album(num[1]);


    char buf[20];
	int tx = 0,ty = 0;
	
	while(1)
	{		
		get_xy(&tx, &ty);//获取触摸屏的坐标
		if(ty > 190 && ty < 290)
		{
			if(tx > 700 && tx < 800)	//右翻
			{
				//printf("你点击的坐标位置X坐标是:%d\n", tx);
				//printf("你点击的坐标位置Y坐标是:%d\n", ty);
				head = head->next;
				bzero(buf,20);
				sprintf(buf,"%s/%s",num[1],head->Data);
			}
			if(tx < 100 && tx > 0)		//左翻
			{
				//printf("你点击的坐标位置X坐标是:%d\n", tx);
				//printf("你点击的坐标位置Y坐标是:%d\n", ty);
				head = head->prev;
				bzero(buf,20);
				sprintf(buf,"%s/%s",num[1],head->Data);
			}
			//showbmp("background.bmp");//开发板下，可执行文件目录下存放的背景图（可以使用绝对路径！）
			//show_1152000bmp(buf);	//显示BMP图片
		}	
		
		if(tx > 700 && ty < 100)
		{
			show_mainUI();
			break;
		}
	}


/*     while (1)
    {
        Get_Touch_Data();
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0) //判断手是否离开
        {
            printf("x:%d--y:%d\n", P_I.x, P_I.y);
            if (P_I.x > 20 && P_I.x < 220 && P_I.y > 300 && P_I.y < 440)
            {
            }

            if (P_I.x > 310 && P_I.x < 480 && P_I.y > 300 && P_I.y < 440)
            {
            }
        }
    } */
}