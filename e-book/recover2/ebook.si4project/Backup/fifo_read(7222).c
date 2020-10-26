#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>


#define FIFO_PATH "/home/china/read.fifo"


int main()
{
	/*
	int ret = mkfifo(FIFO_PATH,0777);
	if(ret < 0)
	{
		perror("mkfifo error");
		return -1;
	}
	*/
	int fifofd = open(FIFO_PATH,O_RDONLY);
	if(fifofd < 0)
	{
		perror("open fifo error");
		return -1;
	}

	char str[256];
	read(fifofd,str,256);
	printf("read fifo is %s\n",str);

	close(fifofd);
}
