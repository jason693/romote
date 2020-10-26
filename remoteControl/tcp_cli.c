#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include<pthread.h>
#include"lcd.h"
#include"yuyv_show.h"
pthread_mutex_t mutex;//定义一把锁的地址
pthread_t pthid;//线程
void *trigger_fun(void *arr);
int main(int argc,char*argv[])
{
    lcd_init(); //初始化屏幕 并映射 
	//第一步：创建套接字 
	int client_sock_fd = socket(AF_INET,SOCK_STREAM,0);
	
	//第二步：请求链接 
	struct sockaddr_in dest_addr;

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(atoi(argv[2]));
	dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	/*ret = bind(client_sock_fd,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
	if(ret<0)
	{
		perror("bind error");
		return -1;
	}
	*/
	ret = connect(client_sock_fd,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
	if(ret<0)
	{
		perror("connect error");
		return -1;
	}
	printf("connect success\n");
	pthread_create(&pthid,NULL,trigger_fun,(void*)&client_sock_fd);//创建一个线程
	pthread_mutex_init(&mutex,NULL);//创建一把锁
	//第三步：收发数据 
	while(1)
	{
		char buf[640*480*2];
		memset(buf,0,sizeof(buf));
		//pthread_mutex_lock(&mutex);
		ret=recv(client_sock_fd,buf,640*480*2,MSG_WAITALL);
		
		if(ret==0)
		{
			//pthread_mutex_unlock(&mutex);
			;
		}
		else
		{
			
			process_yuv_image(buf,640,480); // buf就是缓冲区 缓冲区是哪一个 就是你申请时给的编号
			
		}
		//pthread_mutex_unlock(&mutex);
	}

	//第四步：关闭套接字 
	close(client_sock_fd);
}
void *trigger_fun(void *arr)
{
	int *client_sock_fd=(int*)arr;
	char buf[2]="11";
	while(1)
	{
		//pthread_mutex_lock(&mutex);
		int ret=send(*client_sock_fd,buf,strlen(buf),0);
		//if(ret)
		//pthread_mutex_unlock(&mutex);
	}
}
