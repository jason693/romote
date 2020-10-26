#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

//创建一个单向链表 创建的时候链表为 NULL 所以 同 单向链表的创建 即一个头结点即可

List* create_list(void)
{
	List *list = (List*)malloc(sizeof(list));
	if(list == NULL)
	{
		perror("malloc list error");
		return NULL;
	}
	
	list->frist  = list->last = NULL;
	list->num=0;
	
	return list;
}


//添加节点到链表中 

int add_num_to_list(List* book_list,char *pathname1, char *pathname2)
{
	Node *p = (Node*)malloc(sizeof(Node));
	if(p == NULL)
	{
		perror("malloc node error");
		return -1;
	}
	strcpy(p->book.bmp,pathname1);
	strcpy(p->book.txt,pathname2);
		//插入到链表 
		if(book_list->frist == NULL) //链表为空 既是头也是尾巴 
		{
			book_list->frist = book_list->last=p;
			book_list->num++;
			 
		}
		else //直接尾插
		{
			
				//遍历不能有段错误 即考虑链表为空的情况
				//遍历不可重复
			
				//step1:原尾巴的下一个是新的节点 
					book_list->last->next = p;
				//step2:新节点成为新的尾巴 
					book_list->last = p;
				
					book_list->num++;
			
		}
		

}
	

//请将上述代码写完整 然后 思考如何遍历 注意 不允许使用 for 遍历 list->num的值 

void printf_list(List* list)
{
	Node *r = list->frist;
			
	//遍历不能有段错误 即考虑链表为空的情况
	//遍历不可重复
	while(r)
	{
		printf("%s\n",r->book.bmp);
		printf("%s\n",r->book.txt);
		r=r->next;
	}

}


