#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

struct usebuf{
			void *start; //映射区域的起点地址 
			int length; //映射区域的大小
		};

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
	int ret = connect(client_sock_fd,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
	//第三步：收发数据 
	while(1)
	{
        char buf[640*480*2];
		ret=recv(client_sock_fd,buf,640*480*2,0);
		if(ret<0)
		{
		    continue;
		}
		process_yuv_image(buf,640,480); // buf就是缓冲区 缓冲区是哪一个 就是你申请时给的编号
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

	//第四步：关闭套接字 
	close(client_sock_fd);
}

