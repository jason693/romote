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
	
	list->first  = list->last = NULL;
	list->num=0;
	
	return list;
}


//添加节点到链表中 

int add_num_to_list(List* list,Element num)
{
	Node *p = (Node*)malloc(sizeof(Node));
	if(p == NULL)
	{
		perror("malloc node error");
		return -1;
	}
	
	p->data = num;
	p->next = NULL;
	
	//插入到链表 
	if(list->first == NULL) //链表为空 既是头也是尾巴 
	{
		list->last = list->first = p;
		list->num++;
		list->last->next = list->first; //本身指向了 本身 
	}
	else 
	{
		/*
		//尾插 
			//step1:原尾巴的下一个是新的节点 
				list->last->next = p;
			//step2:新节点成为新的尾巴 
				list->last = p;
			//step3: 新节点的下一个是头
				p->next = list->first; //or list->last->next = list->first;
			
			list->num++;
		//头插 
			//step1:新节点的下一个是原头 
				p->next = list->first;
			//stemp2:新节点成为新的头 
				list->first = p;
			//step3:原尾巴的下一个指向新的头
				list->last->next = list->first;
			list->num++;
			//仅仅比 单向链表多了一句话 list->last->next = list->first;
		//顺序插 
			//解决思路 先找位置 然后插入 头 尾 中间  一毛一样 
		*/
			Node *r = list->first;
			Node *pre = NULL;
			
			//遍历不能有段错误 即考虑链表为空的情况
			//遍历不可重复
			do
			{
				if(r->data > p->data)
				{
					break;
				}
				pre = r;
				r = r->next;
			}while(r!=list->first);
			
			if(r == list->first) 
			{
				if(pre == NULL)  //头插 
				{
					//step1:新节点的下一个是原头 
						p->next = list->first;
					//stemp2:新节点成为新的头 
						list->first = p;
					//step3:原尾巴的下一个指向新的头
						list->last->next = list->first;
					list->num++;
				}
				else //尾插
				{
					//step1:原尾巴的下一个是新的节点 
						list->last->next = p;
					//step2:新节点成为新的尾巴 
						list->last = p;
					//step3: 新节点的下一个是头
						p->next = list->first; //or list->last->next = list->first;
					
					list->num++;
				}
			}
			else  //中间插 
			{
				pre->next = p;
				p->next = r;
				list->num++;
			}	
	}
}

//请将上述代码写完整 然后 思考如何遍历 注意 不允许使用 for 遍历 list->num的值 

void printf_list(List* list)
{
	Node *r = list->first;
			
	//遍历不能有段错误 即考虑链表为空的情况
	//遍历不可重复
	if(r!=NULL)
		do
		{
			printf("%d ",r->data);
			r = r->next;
		}while(r!=list->first);
}

int del_one_numtolist(List* list,Element num)
{
	//第一步：遍历查找 
	Node *r = list->first;
	Node *pre = NULL;
	if(r!=NULL)
	{
		
		do{
			if(r->data == num)
				break;
			pre = r;
			r = r->next;
		}while(r!=list->first);
		/*
		//第二步：分情况考虑删除
		if(r == list->first)
		{
			if(r == list->last)
			{
				r->next = NULL; //本身指向本身的那根线断开
				
				//摘除节点 
				list->first = list->last = NULL;
				list->num--;
				free(r);
				r = NULL;
			}
			else 
			{
				//r的下一个要成为新的头 
				list->first = r->next;
				r->next = NULL;
				//原尾巴的下一个要指向新的头 
				list->last->next = list->first;
				list->num--;
				free(r);
				r = NULL;
			}
		}
		else 
		{
			if( r == list->last)
			{
				//r的前一个成为新的尾巴 
					list->last = pre;
					pre->next = list->first;//or list->last->next = list->first;
					r->next = NULL;
					list->num--;
					free(r);
					r = NULL;
			}
			else 
			{
				pre->next = r->next;
				r->next = NULL;
				list->num--;
				free(r);
				r = NULL;
			}
		}
		*/
		//第二步：分情况考虑删除
		if(r==list->first && pre!=NULL)
		{
			if(r == list->first )
			{
				if(r == list->last)
				{
					//摘除节点 
					list->first = list->last = NULL;
				}
				else 
				{
					//r的下一个要成为新的头 
					list->first = r->next;
					//原尾巴的下一个要指向新的头 
					list->last->next = list->first;
					
				}
			}
			else 
			{
				if( r == list->last)
				{
					//r的前一个成为新的尾巴 
					list->last = pre;
					pre->next = list->first;//or list->last->next = list->first;
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
