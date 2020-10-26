/*
1. ioctl
	文件或设备除了读/写以外，其他控制操作，
	通过ioctl这个函数来完成。


	NAME
	       ioctl - control device

	SYNOPSIS
	       #include <sys/ioctl.h>

	       int ioctl(int fd, int cmd, ...);
	       	fd:文件描述符，表示你要控制哪个文件或设备
	       	cmd: 驱动里面对设备或文件的操作，定义了各
	       		种命令号(是驱动自定义的)，具体命令号的
	       		含义与具体的驱动设计者相关。
	       	... :  执行某个命令操作，可能需要带多个参数，
	       		此处参数与具体的驱动实现相关。
	       返回值:
	       	成功返回0,
	       	失败返回-1,

	
2.



*/


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/videodev2.h>
#include<stdlib.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "lcd.h"
#include "yuyv_show.h"
#include"queue.h"
void *show_image(void *arr);

/*
	用来保存内核中视频缓冲区的信息。
	缓冲区的首地址
	缓冲区的长度

	每一个视频缓冲区都需要对应一个struct vBuf这个结构
*/
struct usebuf{
			void *start; //映射区域的起点地址 
			int length; //映射区域的大小
		};
struct usebuf ub[25] ={0}; //这个N就你申请的一帧数据中有多少张图 因为一张图对应一个缓冲区 
Queue *q;

pthread_mutex_t mutex;//定义一把锁的地址
//pthread_cond_t cond;//定义一个条件变量
pthread_t pthid;//线程



int main(int argc,char*argv[])
{
	int fd; //摄像头的fd 
	int r;
	lcd_init(); //初始化屏幕 并映射 
	q=create_queue();
	/*step 1: 打开摄像头设备*/
	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		perror("open error:");
		return -1;
	}

	/*
	step 2: 查询设备功能(类型)
	*/
	//第一步：获取设备能力信息
	struct v4l2_capability cap;
	int ret = ioctl(fd,VIDIOC_QUERYCAP,&cap); 
	if(ret < 0)
	{
		perror("ioctl VIDIOC_QUERYCAP error");
		return -1;
	}
	printf("cap.capabilities = %d",cap.capabilities);
	if(cap.capabilities == V4L2_CAP_VIDEO_CAPTURE)
	{
		printf("YES\n");
	}


	/*
	step 3: 枚举设备支持的视频格式

	*/
	struct v4l2_fmtdesc check;
	check.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	check.index = 0;
	
	ret = ioctl(fd,VIDIOC_ENUM_FMT,&check);
	if(ret < 0)
	{
		perror("ioctl VIDIOC_ENUM_FMT error");
		return -1;
	}
	printf(" format:%s\n",check.description); //根据输出结构可以知道 视频格式为 YUYV 

	
	/*step 4: 设置设备的视频格式。*/

	struct v4l2_format s_fmt;
	s_fmt.fmt.pix.width = 640; //因为这个摄像头的大小就是 640 * 480 的 
	s_fmt.fmt.pix.height = 480; 
	s_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;//即格式设置为YUYV格式
	s_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	ret = ioctl(fd,VIDIOC_S_FMT,&s_fmt);
	if(ret < 0)
	{
		perror("ioctl VIDIOC_S_FMT error");
		return -1;
	}
	printf("pix.field = %d\n",s_fmt.fmt.pix.field);
	/*
		step 5: 申请视频缓冲
	*/

	struct v4l2_requestbuffers reqbuf;
	memset(&reqbuf,0,sizeof(reqbuf));
	
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.count = 25; //每帧图像数据 最大是 30  1s钟最多获取30张图片 
	reqbuf.memory = V4L2_MEMORY_MMAP;
	
	ret = ioctl(fd,VIDIOC_REQBUFS,&reqbuf);
	if(ret < 0)
	{
		perror("ioctl VIDIOC_REQBUFS error");
		return -1;
	}
	else 
	{
		printf("request buf success\n");
	}

	/*
		step 6: 获取每个缓冲区的信息，并映射，再把它加入到
			采集队列中去。

	*/
	struct v4l2_buffer buf;
	int i = 0;
	for(i=0;i<reqbuf.count;i++)
	{
		memset(&buf,0,sizeof(buf));
		
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i; //i 就是一帧数据中的第几张图片 
		ret = ioctl(fd,VIDIOC_QUERYBUF,&buf);
		if(ret <0)
		{
			perror("ioctl VIDIOC_QUERYBUF error");
			return -1;
		}
		else 
		{
			printf("ioctl VIDIOC_QUERYBUF success\n");
		}
		printf("buf.index = %d,buf.length=%d\n",buf.index,buf.length);
		//第六步：每查到一个内核缓冲 就映射一次
		ub[buf.index].length = buf.length;
		ub[buf.index].start = mmap(NULL,buf.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,buf.m.offset);
		if(ub[buf.index].start == NULL)
		{
			perror("mmap ub[buf.index] error\n");
			return -1;
		}
		else 
		{
			printf("ub[%d].length = %d\n",i,ub[i].length);
		}
		//第七步： 将映射的内存缓冲区 添加到采集队列中
		ret=ioctl(fd,VIDIOC_QBUF,&buf);
		if(ret <0)
		{
			perror("ioctl VIDIOC_QBUF error");
			return -1;
		}
		else 
		{
			printf("ioctl VIDIOC_QBUF success\n");
		}
	}
	
	//查询缓冲区结束后才可以开始采集 
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd,VIDIOC_STREAMON,&type);
	if(ret < 0)
	{
		perror("ioctl VIDIOC_STREAMON error");
		return -1;
	}
	else 
	{
		printf("ioctl VIDIOC_STREAMON success\n");
	}
	
	//设置了一个 IO多路复用来管理fd这一个文件 
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd,&rfds); //将fd加入到了 文件描述符集中 
	/*
        创建互斥锁，和线程
	*/
	pthread_mutex_init(&mutex,NULL);//创建一把锁
	pthread_create(&pthid,NULL,show_image,(void*)NULL);//创建一个线程
	
	while(1)
	{
		int r = select(fd+1,&rfds,NULL,NULL,NULL);
		if(r<=0)
			continue;//失败或者是 超时 再次监听 
		else  //监听到了 
		{
			if(FD_ISSET(fd,&rfds)) //操作  
			{
				//获取一帧数据到视频数据缓冲区中 即 buf中 
				struct v4l2_buffer  vbuf;
				memset(&vbuf,0,sizeof(vbuf));
				vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				vbuf.memory = V4L2_MEMORY_MMAP;
		
				ret = ioctl(fd,VIDIOC_DQBUF,&vbuf);
				if(ret < 0)
				{
					perror("ioctl VIDIOC_DQBUF error");
					return -1;
				}
				else 
				{
					printf("ioctl VIDIOC_DQBUF success\n");
				}
                pthread_mutex_lock(&mutex);
		        //入队
		        ret=Enqueue(q,ub[vbuf.index].start);
		        printf("startaddr=%x\n",ub[vbuf.index].start);
		        //printf("startaddr=%x\n",ub[vbuf.index].start);
		        if(ret < 0)
		        {
		            printf("Failed to join the team");
		            continue;
		        }
		        pthread_mutex_unlock(&mutex);
				//预留在这个位置需要 将视频显示在 lcd屏幕上  xxxx  关键任务的核心 在这个位置 即采集到的数据如何取显示 
				//process_yuv_image(ub[vbuf.index].start,640,480); // buf就是缓冲区 缓冲区是哪一个 就是你申请时给的编号
				//显示结束后 需要将这个缓冲区再次放到任务队列中继续采集 
				usleep(3);
				ret=ioctl(fd,VIDIOC_QBUF,&vbuf);
				if(ret <0)
				{
					perror("ioctl VIDIOC_QBUF error");
					return -1;
				}
				else 
				{
					printf("ioctl VIDIOC_QBUF success\n");
				}
			}
		}
	}
	//停止采集
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd,VIDIOC_STREAMOFF,&type);
	if(ret < 0)
	{
		perror("ioctl VIDIOC_STREAMOD error");
		return -1;
	}
	else 
	{
		printf("ioctl VIDIOC_STREAMOD success\n");
	}
	
	//资源回收 
	for(i = 0;i< 25;i++)
	{
		munmap(ub[i].start,ub[i].length);
	}
	
	close(fd);
	lcd_uninit();
	
}

void *show_image(void *arr)
{
    while(1)
    {
        if(!Queue_IS_Empty(q))
        {
            pthread_mutex_lock(&mutex);
            char data[N];
            printf("111111111111111\n");
            del_queue(q,data);
            printf("2222222222222222\n");
             printf("data=%x\n",data);
            process_yuv_image(data,640,480); // buf就是缓冲区 缓冲区是哪一个 就是你申请时给的编号
            printf("data=%x\n",data);
            pthread_mutex_unlock(&mutex);
            usleep(5);
        }
        else
            usleep(10);
    }
}

