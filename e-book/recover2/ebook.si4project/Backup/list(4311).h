#ifndef _LIST_H_
#define _LIST_H_

typedef char *  Element;
typedef struct node {
		char bmp[10];
		char txt[10];
	struct Node *next;
}Node;

typedef struct head{
	Node *frist;
	Node *last;
	int num;
}List;



List* create_list(void);
int add_num_to_list(List* list,Element pathname1,Element pathname2);
void printf_list(List* list);
int del_one_numtolist(List* list,Element pathname1);
#endif 