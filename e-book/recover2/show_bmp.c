#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "lcd.h"

int show_bmp(char *pathname,int x,int y)
{
	int pic_fd = open(pathname,O_RDWR);
	if (pic_fd < 0)
		{
		perror("open pic error");
		return -1;
		}

	int len,high,depth=0,i,j;

	lseek(pic_fd,0x12,SEEK_SET);
	read(pic_fd,&len,4);
	lseek(pic_fd,0x16,SEEK_SET);
	read(pic_fd,&high,4);
	lseek(pic_fd,0x1C,SEEK_SET);
	read(pic_fd,&depth,2);

	printf("len = %d,high = %d,depth=%d\n",len,high,depth);

	unsigned char buf[len*high*depth/8]; //不对 宽度可能不是4的倍数 注意此时你还是照样读取 根据后续的情况解决

	lseek(pic_fd,54,SEEK_SET);
	int ret = read(pic_fd,buf,len*high*depth/8);
	
	printf("ret = %d\n",ret);
	if(ret == len*high*depth/8)
		printf("success!\n");
	//处理色彩信息 假设 是24位的位图 如何处理  

	int buwei = 0;
	if(len % 4!=0)
		buwei = 4-len*depth/8%4;
	if(depth == 24)
	{
		unsigned char a,r,g,b;
		unsigned char *p = buf;
		int color = 0;
		for(i=y;i<y+high;i++)
		{
			for(j=x;j<x+len;j++)
			{
				a = 0x00;
				b = *(p++);
				g = *(p++);
				r = *(p++);			
				color = a<<24|r<<16|g<<8|b;
				lcd_draw_point(479-i,j,color);
			}
			for(;j<len+buwei;j++)
			{
				p++;
			}
		}
	
	}
	else if(depth == 32)
	{
		unsigned char a,r,g,b;
		unsigned char *p = buf;
		int color = 0;
		for(i=y;i<y+high;i++)
		{
			for(j=x;j<x+len;j++)
			{
				b = *(p++);
				g = *(p++);
				r = *(p++);
				a = *(p++);
				color = a<<24|r<<16|g<<8|b;	
				lcd_draw_point(479-i,j,color);
			}
		}
	}
	else 
	{
		printf("error pic\n");
	}
	close(pic_fd);
	
}

