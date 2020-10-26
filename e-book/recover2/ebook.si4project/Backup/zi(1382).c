#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>


#include "zi.h"
#include "lcd.h"

int zi(int color,int b_color,char *pathname)
{
	//打开书本的路径 失败返回-1
	int book_fd=open(pathname,O_RDONLY);
	if(book_fd < 0)
	{
		perror("open book error");
		return -1;
	}

	//定义显示文字的开始坐标
	int ret=0,x=50;
	do
	{
		int y=10;
		//定义一个字符数组初始化为0 用于存储从文件中读取的字符
		char string[1024]={0};
		
		//定义一个指向字符串数组的指针
		char * p=string;
		
		//从文件中读取字符到数组中 失败返回-1
		ret  =read(book_fd,string,1024);
		printf("ret =%d\n",ret);
		if(ret < 0)
		{
			perror("read book error");
			return -1;
		}

		//n_c英文的个数 n_z中文的个数
		int n_c = 0,n_z = 0;
		
		//行数
		int line_num = 0;
		
		int i1=0,j=0,i=0;
		//当string[i]!='\0'即不是字符的末尾
		while(string[i] != '\0')
		{
			
			if(n_z+n_c==800)//一行容纳不下，换行		中文字数+英文字数== 一行的字数(自己设置的字数)
			{
				n_z = n_c = 0;//将字数都置为0 
				line_num++;//另起一行
			}
			//判断是不是\n换行
			if(string[i]=='\n')
			{	
				x=x+20;
				y=10;
				i=i+1;
			}

			if(string[i]==' ')
			{
				y=y+8;
				i=i+1;
			}
			//判断是不是中文字符 为真表示是中文字符
			if(string[i] & 0x80)
			{
			//中文
				int start = ((string[i]-0xa1)*94+(string[i+1]-0xa1))*32+128*16;//中文字符的起始字节数据
				
			
				for(i1 = 0;i1 < 32;i1++)//中文是32个字节
				{
					for(j = 0;j <8;j++)//一个像素像素的读8位
					{
						if(character[start+i1] >>j & 0x01)
						{
							lcd_draw_point(x+i1/2,y+(i1%2)*8+(7-j),color);
						}
						else
						{
							//不是就置另外的颜色
							lcd_draw_point(x+i1/2,y+(i1%2)*8+(7-j),b_color);				
						}									
				   }
				
				}
				y=y+8;
				i+=2;//中文是两字节
				n_z++;//中文字数加1				
			}				
			else
			{
			//英文	
				printf("string=%c\n",string[i]);
				
				int start = string[i] *16;
				//i++;//英文占一个字节
				for(i1 = 0;i1 < 16;i1++)//高度不能大于16个像素点
				{
					for(j = 0;j <8;j++)
					{
						if(character[start+i1]>>j & 0x01)
						{
		
							lcd_draw_point(x+i1,y+(7-j),color);
							
						}						
						else
						{
							lcd_draw_point(x+i1,y+(7-j),b_color);
						}										
					}
				}	
				i=i+1;	
					n_c++;//英文字数加1
			}
			y=y+8;
					
		}
		
		x=x+20;
	}while(ret==1024);
}





