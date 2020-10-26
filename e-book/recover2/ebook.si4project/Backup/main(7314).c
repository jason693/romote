#include <stdio.h>


#include "lcd.h"
#include "show_bmp.h"
//#include "list.h"
#include "touch.h"
#include "zi.h"
//屏幕信息读取、图片显示、链表功能、触屏功能、点阵字库判断

int main()
{
	//第一步：打开屏幕
	lcd_open();

	while(1)
	{
		int k=0;
		//第二步：显示书架及书架上的书本封面
		//1、显示书架
		loop:show_bmp("./bookshelf.bmp",0,0);
		
		//2、显示书架上的图书封面
		//1) 定义一个字符数组，用于存储图书封面的路径
		char *book_cover[10]={"./1.bmp","./2.bmp","./3.bmp","./4.bmp","./5.bmp",
							"./6.bmp","./7.bmp","./8.bmp","./9.bmp","./10.bmp"};
		/*
			2)判断图书封面的放置位置
				设置成一个书架放8本书
				一层放4本
				可以翻页
				则要定义一个数组来存放要放的位置的横纵坐标
		*/
		
		int x_axis[4] = {60,260,460,660};//一层4本书的横坐标
		int y_axis[2] = {60,270};//两层
		int i,j;
		for(j=0;j<2;j++)
		{
			for(i=0;i<4;i++)
			{
				show_bmp(book_cover[k],x_axis[i],y_axis[j]);
				k++;
				
				
			}
			if(k==9)
					break;
		}
		
		//满8本即满页了 翻页
		//第三步：获取触屏功能
		touch_open();
		struct point ret = get_touch_with_point();
		if(ret.x > 1000 && ret.x < 1200 && ret.y > 0 && ret.y <480)
		{	goto loop;

		}
		else if( ret.x > 80 && ret.x < 167 && ret.y > 84 && ret.y <260)
		{
		
			zi(WHITE,BLACK);//进入第一本书
		}
		else if(ret.x > 329 && ret.x < 427 && ret.y > 84 && ret.y <260)
		{
			
		}
		else if(ret.x > 588 && ret.x < 677 && ret.y > 84 && ret.y <260)
		{

		}
		else if(ret.x > 856 && ret.x < 940 && ret.y > 84 && ret.y <260)
		{

		}//第一层的书else 
		else if(ret.x > 80 && ret.x < 167&& ret.y > 334 && ret.y <510)
		{

		}
		else if(ret.x > 329 && ret.x < 427&& ret.y > 334 && ret.y <510)
		{
		
		}
		else if(ret.x > 588 && ret.x < 677&& ret.y > 334 && ret.y <510)
		{
		
		}
		else if(ret.x > 856 && ret.x < 940  && ret.y > 334 && ret.y <510)
		{
		
		}
		
	}
	
}
	
		
		
		

