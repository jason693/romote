#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

#include "zi.h"
#include "lcd.h"


int zi(int x,int y,int color,int b_color)
{
	char string[] = "你好ZhongGuo";//用函数获取.txt文件 存放到string 字符数组中
	unsigned char data;
	int n_c = 0,n_z = 0;
	int line_num = 0;//行数
	char *p=NULL;
	int n,m,i=0;
	while(string[i] != '\0')
	{
		if(n_z+n_c == 2)//一行容纳不下，换行		中文字数+英文字数== 一行的字数(自己设置的字数)
		{
			n_z = n_c = 0;//将字数都置为0 
			line_num++;//另起一行
		}
	p = n_z*16+n_c*8+line_num*800;//org_row*800+org_colum//(0,0)为原点
	if(string[i] & 0x80)
	{
		//中文
		int position = ((string[i]-0xa1)*94+(string[i+1]-0xa1))*32+128*16;//中文字符的起始字节数据
		i+=2;//中文是两字节
		for(m = 0;m < 16;m++)//高度不能大于16个像素点
		{
			data = c[position+m*2];//从点阵字符数组中找到 对应的字
			for( n = 0;n <8;n++)//左边  (前8个像素点)
			{
				
				if(data & (0x80 >> n))
				{
					//lcd_draw_point(x+m/2,y+(m%2)*8+(7-n),color);
					*((int *) p) = color;//是点阵的位置就置颜色
				}
				else
				*((int *) p) = b_color;//不是就置另外的颜色
			}
			data = c[position+m*2+1];
			for(n = 0;n <8;n++)//右边	(后8个像素点)
			{
				if(data & (0x80 >> n))
				{
					*((int *) p) = color;
				}
				else
					*((int *) p) = b_color;
			}
		}
		n_z++;//中文字数加1
	}
	else
	{
		//英文
		int position = string[i] *16;
		i++;//英文占一个字节
		for(m = 0;m < 16;m++)//高度不能大于16个像素点
		{
			data = c[position+m];
			for(n = 0;n <8;n++)
			{
				if(data & (0x80 >> n))
				{
					//lcd_draw_point(x,y,color);
					//printf("g");
					*((int *)p) = color;
				}
				else
					*((int *)p) = b_color;
			}
			
		}
		n_c++;//英文字数加1
	}
	y++;
}
	return 0;
}


