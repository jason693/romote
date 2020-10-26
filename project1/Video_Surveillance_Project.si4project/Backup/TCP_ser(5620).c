#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include"queue.h"
#include <pthread.h>

extern Queue *q1;
extern pthread_mutex_t mutex;//定义一把锁的地址

void *tcp_ser(void *arr)
{
    printf("00000000000000000000\n");
    fd_set fdsetp,copy_fdsetp;
	//fdsetp: 用于作为select调用钱检测是否可读的文件描述符结合 
	//copy_fdsetp 用于调用select 返回后就绪的文件描述符集合

	int fd_max;//最大的文件描述符+1 的值 
	int i; 
	int conn_sock; //connect的套接字描述符 	
	//第一步：创建套接字 必须是流式套接字 
	int ser_sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(ser_sock_fd < 0)
	{
		perror("socket error");
		return NULL;
	}
	//第二步：绑定地址和端口号 
	char *str="10086";
	struct sockaddr_in ser_addr;

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(atoi(str));
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(ser_sock_fd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(ret <0)
	{
		perror("bind error");
		return NULL;
	}
	//第三步：监听

	ret = listen(ser_sock_fd,10);
	if(ret <0)
	{
		perror("listen error");
		return NULL;
	}

	//清空两个集合 
	FD_ZERO(&fdsetp);
	FD_ZERO(&copy_fdsetp);
	//将监听的 文件描述符 添加到 集合中 
	FD_SET(ser_sock_fd,&fdsetp);
	
	fd_max = ser_sock_fd+1;
	while(1)
	{
        printf("888888888888888888\n");
		struct timeval timeout;
		//监听超时时间设置 20s 
		timeout.tv_sec = 20;
		timeout.tv_usec = 0;

		copy_fdsetp = fdsetp;
		int sec_ret = select(fd_max,&copy_fdsetp,NULL,NULL,&timeout); 
		//第一次进来只监听一个 套接字描述符 就是服务端的套接字 
		//如果的服务端的套接字可读 一定是有连接进来了 

		//如果第二次进来还是要建立连接的 那么  i的值 一定再 集合中 并且 i应该就是服务端的套接字 
		// 同第一次 
		//如果第二次 进来的 不是 建立连接 而是客户端发来的消息 那么第一次 添加进入描述符集合的那个
		//套接字就是可读的 
		if(sec_ret == -1)
		{
			perror(" select error");
			return NULL;
		}
		else if(sec_ret == 0)
		{
			printf("time out ....\n");
			continue;
		}
		else 
		{
			for(i = 0;i<fd_max;i++) //找所有的描述符 
			{
				//从 0判断 看是不是再 我监听的描述符集合中 
				if(FD_ISSET(i,&copy_fdsetp)) //如果为真 则 这个文件描述符是可读的 
				{	//成立表示在
					if(i == ser_sock_fd) //描述符 就是 服务端的套接字描述符 
						//请求连接 
					{
							//第四步：接受链接 //允许客户端链接
						struct sockaddr_in sa;
						bzero(&sa,sizeof(sa));
						int len = sizeof(sa);
						conn_sock = accept(ser_sock_fd,(struct sockaddr*)&sa,&len);
						if(conn_sock < 0)
						{
							perror("accept error");
							return NULL;
						} 
						else 
						{
							printf("accept success\n");
							fd_max = fd_max > conn_sock+1 ?fd_max:(conn_sock+1);
							//新建立的 链接后 的套接字描述符也是 本程序的一个描述符 
							//如果他比之前的要大则 这个就是现在最大的 
							//如果没有之前的大 则 以之前为准 因为还是能够监听到 
							FD_SET(conn_sock,&fdsetp);
							//来了一个新的描述符 就是要通信 既然通信则加入到监听的描述符集合中
						}
					}
					else 
					{
						////第五步：收发数据 
						if(!Queue_IS_Empty(q1))
                        {
                            while(!Queue_IS_Empty(q1))
                            {
                                pthread_mutex_lock(&mutex);
                                char data[N];
                                del_queue(q1,data);
                                ret = send(i,data,strlen(data),0);
                                if(ret<0)
                                {
                                    perror("send error");
                                    pthread_mutex_unlock(&mutex);      
                                    continue;
                                }
                                pthread_mutex_unlock(&mutex);      
                             }
                        }
                        else
                         usleep(30);

				    }
			    }
		    }
	    }
	}
	//第六步：关闭链接 
	close(ser_sock_fd);
}
