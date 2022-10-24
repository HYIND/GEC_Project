#ifndef _FD_H_
#define _FD_H_

#include <stdio.h>

extern int fd_lcd;
extern unsigned int *p_lcd;
extern int fd_ts;

void Init_fd();
void close_fd();

#endif