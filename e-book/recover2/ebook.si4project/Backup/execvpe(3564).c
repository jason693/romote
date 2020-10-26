#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main()
{
	printf("I am main\n");

	pid_t pid = fork();
	if(pid <0)
	{

	}
	else if(pid > 0)
	{
		printf("I am father,my pid = %d,my father pid= %d\n",getpid(),getppid());
		int status;
		pid_t ret_pid = wait(&status);
		printf("return pid = %d\n",ret_pid);
		printf("return num = %d\n",WEXITSTATUS(status));
		exit(0);
	}
	else 
	{
		printf("I am son,my pid = %d,my father pid= %d\n",getpid(),getppid());
		/* char * const envp[]
			请问这是什么？ 数组 存储的是 指针 char* 
				指针数组 
				请问此处 const的意思是什么？ 
					const int* p; //修饰的是谁？ p的指向 即p指向的内容不可以改 
					int *const p; //修饰的是谁？ p 即p的值不可以改 
					const int *const p; //修饰的是谁？ p 与 p的指向 都不可以修改 
			char * const envp[]; const 修饰的是谁？ 即数组成员的值不可以改 		

		*/
		char *arg[] = {"./main","123","456","abc","def",NULL};
		char *const envp[]={"AA=12","BB=don’tbb","SS=tt",NULL}; //注意环境变量也要写 NULL 
		int ret = execvpe("main",arg,envp);
		if(ret <0)
		{
			perror("execl error");
			return -1;
		}
		printf("end\n");
	}
}


