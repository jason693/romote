#ifndef _LIST_H_
#define _LIST_H_

typedef char * Element;
typedef struct node {
		char bmp[10];
		char txt[10];
	sturct node next;
}Node;

typedef struct head{
	Node *frist;
	Node *last;
	int num;
}List;


#endif 