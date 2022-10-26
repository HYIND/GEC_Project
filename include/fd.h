#ifndef __FD_H__
#define __FD_H__

#include <stdio.h>

extern int fd_lcd;
extern unsigned int *p_lcd;
extern int fd_ts;

void Init_fd();
void close_fd();

#endif