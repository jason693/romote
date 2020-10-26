#ifndef _LIST_H_
#define _LIST_H_


typedef struct information{
		char bmp[10];
		char txt[10];
}Information;

typedef struct node {
	Information book;
	struct node *next;
}Node;

typedef struct head{
	Node *frist;
	Node *last;
	int num;
}List;



List* create_list(void);
int add_num_to_list(List* book_list,char *pathname1, char *pathname2);
void printf_list(List* list);

#endif 