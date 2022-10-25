#include "album.h"

P_Node cur_node;    //当前正在访问的图片所在链表节点

P_Node New_Node(data_type num)
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

void Init_Album()
{
    //读取目录下的图片，生成双向链表
}

void Album()
{
    //初始化相册信息
    Init_Album();

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