#include "fd.h"
#include <stdio.h>
#include <memory.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "show_bmp.h"

int fd_lcd = -1;
unsigned int *p_lcd = NULL;

int fd_ts = -1;

void Init_lcd()
{
    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd == -1)
    {
        printf("open fd_lcd fail!");
        exit(1);
    }

    p_lcd = mmap(NULL, 800 * 480 * 8, PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd_lcd, 0);
    if (p_lcd == NULL)
    {
        perror("mmap error");
        exit(1);
    }
}

void Init_touch()
{
    // open event0
    fd_ts = open("/dev/input/event0", O_RDWR | O_APPEND);
    if (fd_ts == -1)
    {
        printf("打开触摸屏失败！\n");
        exit(1);
    }
}

void Init_fd()
{
    Init_lcd();
    Init_touch();
}

void close_fd()
{
    close(fd_lcd);
    close(fd_ts);
}