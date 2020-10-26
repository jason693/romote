#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#include "touch.h"

int ts_fd = -1;

int  touch_open(void)
{
	ts_fd =open("/dev/input/event0",O_RDWR);
	if(ts_fd < 0)
	{
		return -1;
	}
	return 0;
}

int touch_close(void)
{
	return close(ts_fd);
}


int get_touch(void)
{
	struct input_event in;

	while(1)
	{
		if(read(ts_fd,&in,sizeof(in))== sizeof(in)) //读取成功
		{
			//结构体解析
			if(in.type == EV_KEY && in.code == 0x14a) //表示有压力值
			{
				printf("type = %d,code = %d,value = %d\n",in.type,in.code,in.value);
				if(in.value > 0)
				{		//按下 记录
					continue;
				}
				else
				{
					break;
				}
						
			}
		}
	}
	return 1;
}

struct point get_touch_with_point(void)
{
	struct input_event in;
	start.x = start.y =end.x = end.y = -1; // 

	struct point temp;
	while(1)
	{
		if(read(ts_fd,&in,sizeof(in))== sizeof(in)) //读取成功
		{
			//结构体解析
			if(in.type == EV_ABS && in.code == ABS_X)
			{
				temp.x = in.value;
			}
			else if(in.type == EV_ABS && in.code == ABS_Y)
			{
				temp.y = in.value;
			}
			else if(in.type == EV_KEY && in.code == 0x14a) //表示有压力值
			{
				printf("type = %d,code = %d,value = %d\n",in.type,in.code,in.value);
				if(in.value > 0)
				{		//按下 记录
					if(start.x == -1 && start.y == -1)
					{
						start.x = temp.x;
						start.y = temp.y;
					}
					continue;
				}
				else
				{
					end.x = temp.x;
					end.y = temp.y;
					break;
				}
						
			}
		}
	}
	printf("start(%d,%d),end(%d,%d)\n",start.x,start.y,end.x,end.y);
	
	return end;
}


