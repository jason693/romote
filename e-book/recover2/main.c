#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lcd.h"
#include "show_bmp.h"
#include "list.h"
#include "touch.h"
#include "zi.h"
//屏幕信息读取、图片显示、链表功能、触屏功能、点阵字库判断

List *to_get_catalog(void);




int main()
{
	//第一步：打开屏幕
	lcd_open();
	
	List *p =create_list(); ;
	
	p=to_get_catalog();
	Node *l = (Node *)malloc(sizeof(Node));
	while(1)
	{
	
		loop2:l=p->frist;
		
			int k =0;
			
		loop:show_bmp("./bookshelf.bmp",0,0);
		int x_axis[4] = {60,260,460,660};//一层4本书的横坐标
		int y_axis[2] = {60,270};//两层
		int i,j;

		for(j=0;j<2;j++)
			{
				for(i=0;i<4;i++)
				{
						if(k==10)
							goto loop1;
						if(l == NULL)
							goto loop2;//等于空
						show_bmp(l->book.bmp,x_axis[i],y_axis[j]);
						k++;
						l=l->next;
						
				}
				
			}
			
		
		loop1:touch_open();
		struct point ret = get_touch_with_point();
		if(ret.x > 1000 && ret.x < 1200 && ret.y > 0 && ret.y <480)
		{
			if(k < 9)
			{
				goto loop;
			}

		}
		else if( (ret.x > 80 && ret.x < 167 && ret.y > 84 && ret.y <260)&&k<9)
		{
			
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./1.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
			
	
		}
		else if((ret.x > 329 && ret.x < 427 && ret.y > 84 && ret.y <260)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./2.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 588 && ret.x < 677 && ret.y > 84 && ret.y <260)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./3.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 856 && ret.x < 940 && ret.y > 84 && ret.y <260)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./4.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);

		}//第一层的书else 
		else if((ret.x > 80 && ret.x < 167&& ret.y > 334 && ret.y <510)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./5.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 329 && ret.x < 427&& ret.y > 334 && ret.y <510)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./6.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 588 && ret.x < 677&& ret.y > 334 && ret.y <510)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./7.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 856 && ret.x < 940  && ret.y > 334 && ret.y <510)&&k<9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./8.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 80 && ret.x < 167&& ret.y > 334 && ret.y <510)&&k>9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./9.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
		else if((ret.x > 329 && ret.x < 427&& ret.y > 334 && ret.y <510)&&k>9)
		{
			show_bmp("./book.bmp",0,0);
			Node* r=find_list(p,"./10.bmp");
			printf("r=%s\n",r->book.txt);
			zi(BLACK,WHITE,r->book.txt);
		}
			
	}

	
		

}
	

	
		
List *to_get_catalog(void)//将当前目录下所有的.bmp文件和.txt文件一一对应 存储到链表中
{
	List *path = (List*)malloc(sizeof(List));
	path = create_list();
	char *book_cover[10]={"./1.bmp","./2.bmp","./3.bmp","./4.bmp","./5.bmp",
								"./6.bmp","./7.bmp","./8.bmp","./9.bmp","./10.bmp"};
	char *book_in[10]={"./1.txt","./2.txt","./3.txt","./4.txt","./5.txt",
						"./6.txt","./7.txt","./8.txt","./9.txt","./10.txt"};
	int i=0;
	for(i=0;i<10;i++)
	{
		add_num_to_list(path,book_cover[i],book_in[i]);
		printf("%s\n",path->frist->book.bmp);
	
	}
	return path;
}
		

