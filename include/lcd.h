#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "jpeglib.h"

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)

unsigned long file_size_get(const char *pfile_path);
int lcd_open(const char *str);
void close_lcd(void);
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color);
int lcd_draw_bmp(unsigned int x,unsigned int y,const char *pbmp_path);