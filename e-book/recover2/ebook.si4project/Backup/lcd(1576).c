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

int lcd_open(void)
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
		perror("mmap error");
		return -1;
	}
	return 0;
}


int lcd_close(void)//关闭屏幕
{
	//解除映射
	int ret = munmap(plcd,lcd_info.xres*lcd_info.yres*lcd_info.bits_per_pixel/8);
	if(ret < 0)
	{
		perror("munmap error");
		return -1;
	}
	//关闭屏幕
	ret = close(lcd_fd);
	if(ret < 0)
	{
		perror("close error");
		return -1;
	}
}

//画点函数、画块函数、画圆函数
void lcd_draw_point(int x,int y,int color)
{
	*(plcd+x*800+y) = color;
}

//x是行坐标 y是列坐标
void lcd_draw_block(int x, int y,int length,int high, int color)
{
	int i,j;
	for(i=x;i<x+high;i++)
	{
		for(j=y;j<y+length;j++)
		{
			lcd_draw_point(i,j,color);
		}
	}
}

//满圆
void lcd_draw_full_circle(int x, int y, int r,  int color)
{
	int i,j;
	for(i=0;i<lcd_info.yres;i++)
	{
		for(j=0;j<lcd_info.xres;j++)
		{
			if((i-x)*(i-x)+(j-y)*(j-y)<r*r)//圆的方程
			lcd_draw_point(i, j, color);
		}	
	}
}

//空心圆
void lcd_draw_empty_circle(int x, int y, int r, int color)
{
	int i,j;
	for(i=0;i<lcd_info.yres;i++)
	{
		for(j=0;j<lcd_info.xres;j++)
		{
			if((i-x)*(i-x)+(j-y)*(j-y) < r*r && (i-x)*(i-x)+(j-y)*(j-y) > (r-5)*(r-5))
			lcd_draw_point(i, j, color);	
		}
	}
}

void draw_word(unsigned char *name,int x,int y,int len,int size,int color)
{
	int flag = len/8;

	int i,j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<8;j++)
		{
			if(name[i]>>j & 0x01)
				lcd_draw_point(x+i/flag,y+(i%flag)*8+(7-j),color);
		}
	}
}

