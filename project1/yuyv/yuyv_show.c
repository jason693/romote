/*

	把一个jpeg图片文件解压并显示在
	屏幕上。

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jpeglib.h>
#include<sys/mman.h>
#include<string.h>
#include<fcntl.h>
#include "lcd.h"

int convert_yuv_to_rgb_pixel(int y, int u, int v)
{
	 unsigned int pixel32 = 0;
	 unsigned char pixel[3];// = (unsigned char *)&pixel32;
	 int r, g, b;
	 r = y + (1.370705 * (v-128));
	 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
	 b = y + (1.732446 * (u-128));
	 if(r > 255) r = 255;
	 if(g > 255) g = 255;
	 if(b > 255) b = 255;
	 if(r < 0) r = 0;
	 if(g < 0) g = 0;
	 if(b < 0) b = 0;
	 pixel[0] = r * 220 / 256;
	 pixel[1] = g * 220 / 256;
	 pixel[2] = b * 220 / 256;

	 pixel32 = (pixel[0] << 16 ) | (pixel[1] << 8) | (pixel[2]);
	 return pixel32;
}
/*
	函数功能：将 yuyv格式 转化为 argb格式 
	参数： yuv 即存储yuyv格式图像数据的数组 这个数组 是一个 unsigned char * 即映射区域的首地址 
			映射首地址存储在 ub[] 中成员 start 中 
			width 视频图像的宽度 
			height 视频图像的高度 
	返回值： 无
*/
void process_yuv_image(unsigned char *yuv, int width, int height)
{
	unsigned int in, out = 0;
	unsigned int pixel_16;
	unsigned char pixel_24[3];
	unsigned int pixel32;
	 int y0, u, y1, v;

	int pixel_num = 0;

 	for(in = 0; in < width * height * 2; in += 4) // 640 *480 *2 就是数组的总大小  每4个一组
	{
		//四字节组合成一个int型数据 
		pixel_16 =
			yuv[in + 3] << 24 |
			yuv[in + 2] << 16 |
			yuv[in + 1] <<  8 |
			yuv[in + 0];
		//将几个数据分量提取出来  y0在最低字节 u在 次低字节 y1在次高字节 V在 高字节 
		y0 = (pixel_16 & 0x000000ff);
		u  = (pixel_16 & 0x0000ff00) >>  8;
		y1 = (pixel_16 & 0x00ff0000) >> 16;
		v  = (pixel_16 & 0xff000000) >> 24;
		
		//通过y0 u v 计算出颜色数据  这里算的是第一个点 
 	 	pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
		pixel_num ++;
		//画点函数 
		lcd_draw_point(pixel_num/width,pixel_num%width,pixel32);

		
		//pixel_24[0] = (pixel32 & 0x000000ff);
		//pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
		//pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
		//rgb[out++] = pixel_24[0];
		//rgb[out++] = pixel_24[1];
		//rgb[out++] = pixel_24[2];
		//这里算的是第二个点 
		pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
		pixel_num ++;
		lcd_draw_point(pixel_num/width,pixel_num%width,pixel32);
		//pixel_24[0] = (pixel32 & 0x000000ff);
		//pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
		//pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
		//rgb[out++] = pixel_24[0];
		//rgb[out++] = pixel_24[1];
		//rgb[out++] = pixel_24[2];
 	}
	printf("test 127 \n");
	 return ;
}

