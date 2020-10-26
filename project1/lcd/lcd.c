#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

static int lcd_fd = -1; //屏幕文件描述符 
static int *plcd = NULL; //映射首地址 
static struct fb_var_screeninfo lcd_info;

int lcd_init(void)
{
	//第一步：打开屏幕 
	lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd < 0)
	{
		perror("open lcd error");
		return -1;
	}

	//第二步：获取屏幕属性 
	
	int ret = ioctl(lcd_fd,FBIOGET_VSCREENINFO,&lcd_info);
	if(ret < 0)
	{

	}

	printf("lenth = %d,high = %d\n",lcd_info.xres,lcd_info.yres);

	printf("color depth = %d\n",lcd_info.bits_per_pixel);

	printf("red.offset=%d,red.length=%d,red.msb_right = %d\n",lcd_info.red.offset,
			lcd_info.red.length,lcd_info.red.msb_right);
	printf("green.offset=%d,green.length=%d,green.msb_right = %d\n",lcd_info.green.offset,
			lcd_info.green.length,lcd_info.green.msb_right);
	printf("blue.offset=%d,blue.length=%d,blue.msb_right = %d\n",lcd_info.blue.offset,
			lcd_info.blue.length,lcd_info.blue.msb_right);
	//第三步：映射屏幕
	plcd = (int *)mmap(NULL,lcd_info.xres*lcd_info.yres*lcd_info.bits_per_pixel/8,
	PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(plcd == NULL)
	{

	}
	return 0;
}

int lcd_uninit(void)
{
	//解除映射 
	int ret = munmap(plcd,lcd_info.xres*lcd_info.yres*lcd_info.bits_per_pixel/8);
	if(ret < 0)
		{

		}
	//关闭屏幕
	ret = close(lcd_fd);
	if(ret < 0)
		{

		}
}

//请实现以下的几个函数 
/*
	1、画点函数 
		即在屏幕上画一个点 这个点的坐标假设是第i行 第j列 并且颜色显示为color
		函数的 参数有三个 坐标  i j 和点的颜色 
	2、画块函数 
		即再屏幕上画一个矩形 这个矩形 
			起点坐标 i j 宽 len high 矩形的颜色 
	3、画圆函数 
		即屏幕上画一个 圆圈 
			圆形坐标 和半径
		分为两种 
			实心圆 圆的方程 应该是 小于这个条件  
			和空心圆  圆的方程  小于 并且大于某个 
*/
/*
	函数原型：void lcd_draw_point(int x,int y,int color)
	函数功能：在屏幕上画一个指定位置的点 
	参数： 
		x 该点的行坐标 
		y 该点的列坐标
*/
void lcd_draw_point(int x,int y,int color)
{
	if(x>=0 && x <480 && y>=0 && y<800)
		*(plcd + x*800 +y) = color;
}

/*
	函数原型： void lcd_draw_block(int x0,int y0,int lenth,int high,int color)
	函数功能：画块函数
	参数： 
		x0 起点的行坐标 
		y0 起点的列坐标 
		lenth 宽度 
		high 高度
		color 颜色 
*/
void lcd_draw_block(int x0,int y0,int lenth,int high,int color)
{
	int i,j;
	for(i=x0;i<x0+high;i++)
	{
		for(j=y0;j<y0+lenth;j++)
		{
			lcd_draw_point(i, j, color);
		}
	}
}

/*
	函数原型：
	函数功能：
	函数参数：
*/

void lcd_draw_full_circle(int x0,int y0,int r,int color)
{
	int i,j;
	for(i=0;i<lcd_info.yres;i++)
	{
		for(j=0;j<lcd_info.xres;j++)
		{
			if((i-x0)*(i-x0)+(j-y0)*(j-y0) < r*r)
			{
				lcd_draw_point(i, j, color);
			}
		}
	}
}


void lcd_draw_empty_circle(int x0,int y0,int r,int color)
{
	int i,j;
	for(i=0;i<lcd_info.yres;i++)
	{
		for(j=0;j<lcd_info.xres;j++)
		{
			if((i-x0)*(i-x0)+(j-y0)*(j-y0) < r*r && (i-x0)*(i-x0)+(j-y0)*(j-y0) > (r-5)*(r-5))
			{
				lcd_draw_point(i, j, color);
			}
		}
	}
}

void draw_word(unsigned char *name,int x0,int y0, int len,int size,int color)
{
	int flag = len/8; //即 字的宽度使用几个字节的数据表示 
	
	int i,j;
	
	for(i=0;i<size;i++) //遍历每一个字节 
	{
		for(j=0;j<8;j++) //遍历每一个字节的每一位 
		{
			if(name[i]>>j & 0x01)  //为真表示这一位为1 			
			{
				lcd_draw_point(x0+i/flag,y0+(i%flag)*8+(7-j),color);
			}
		}
	}
}


void show_yuyv_rgb(char *buf)
{
	int i,j;
	char a, r,g,b;
	int color;
	for(i=0;i<480;i++)
	{
		for(j=0;j<640;j++)
		{
			a=0x00;
			r = buf[(i*640+j)*3];
			g = buf[(i*640+j)*3 +1];
			b = buf[(i*640+j)*3 +2];
			color = (a<<24)|(r << 16) | (g << 8) | b;
			lcd_draw_point(i,j,color);
		}
	}
}



