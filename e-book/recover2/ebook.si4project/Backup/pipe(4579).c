#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	printf("I am main\n");
	int pipefd[2] ={0};
	int ret = pipe(pipefd);
	if(ret < 0)
	{
		perror("pipe create error");
		return -1;
	}
	
	pid_t pid = fork(); //之后子进程 也拷贝了 pipefd 这个数组 即有了共同管道的文件描述符
	if(pid < 0)
	{
		perror("fork error");
		return -1;
	}
	else if(pid > 0) //父
	{
		printf("pid = %d\n my pid = %d\n my father pid = %d\n",pid,getpid(),getppid());

		int ret = write(pipefd[1],"I am your father!\n",strlen("I am your father!\n"));
		printf("write ret = %d\n",ret);

		int status;
		pid_t exit_pid = wait(&status);
		printf("my son %d is died\n",exit_pid);
	}
	else //子
	{
		printf("pid = %d\n my pid = %d\n my father pid = %d\n",pid,getpid(),getppid());
		char str[256] = {0};
		int ret = read(pipefd[0],str,256);
		printf("read ret = %d,str = %s\n",ret,str);
		if(ret < 0)
			exit(-1);
		else 
			exit(0);
	}
	
}

