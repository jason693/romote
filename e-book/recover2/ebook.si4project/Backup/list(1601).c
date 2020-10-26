#include <stdio.h>
#include <stdlib.h>


//创建一个单向循环链表 创建的时候链表为 NULL 所以 同 单向链表的创建 即一个头结点即可

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

int add_num_to_list(List* list,Element pathname1,Element pathname2)
{
	Node *p = (Node*)malloc(sizeof(Node));
	if(p == NULL)
	{
		perror("malloc node error");
		return -1;
	}
	
	p->bmp = pathname1;
	p->txt = pathname2;
	p->next = NULL;
	
	//插入到链表 
	if(list->frist == NULL) //链表为空 既是头也是尾巴 
	{
		list->last = list->frist = p;
		list->num++;
		list->last->next = list->frist; //本身指向了 本身 
	}
	else //直接尾插
	{
		
			Node *r = list->frist;
			Node *pre = NULL;
			
			//遍历不能有段错误 即考虑链表为空的情况
			//遍历不可重复
		
			//step1:原尾巴的下一个是新的节点 
				list->last->next = p;
			//step2:新节点成为新的尾巴 
				list->last = p;
			//step3: 新节点的下一个是头
				p->next = list->frist; //or list->last->next = list->first;
			
				list->num++;
		
	}
}

//请将上述代码写完整 然后 思考如何遍历 注意 不允许使用 for 遍历 list->num的值 

void printf_list(List* list)
{
	Node *r = list->frist;
			
	//遍历不能有段错误 即考虑链表为空的情况
	//遍历不可重复
	if(r!=NULL)
		do
		{
			printf("%s ",r->bmp);
			r = r->next;
		}while(r!=list->frist);
}

int del_one_numtolist(List* list,Element pathname1)
{
	//第一步：遍历查找 
	Node *r = list->frist;
	Node *pre = NULL;
	if(r!=NULL)
	{
		
		do{
			if(r->bmp == pathname1)
				break;
			pre = r;
			r = r->next;
		}while(r!=list->frist);
		
		//第二步：分情况考虑删除
		if(r==list->frist && pre!=NULL)
		{
			if(r == list->frist )
			{
				if(r == list->last)
				{
					//摘除节点 
					list->frist = list->last = NULL;
				}
				else 
				{
					//r的下一个要成为新的头 
					list->frist = r->next;
					//原尾巴的下一个要指向新的头 
					list->last->next = list->frist;
					
				}
			}
			else 
			{
				if( r == list->last)
				{
					//r的前一个成为新的尾巴 
					list->last = pre;
					pre->next = list->frist;//or list->last->next = list->first;
				}
				else 
				{
					pre->next = r->next;
				}
			}
			r->next = NULL;
			list->num--;
			free(r);
			r = NULL;
			
		}
	}
}
