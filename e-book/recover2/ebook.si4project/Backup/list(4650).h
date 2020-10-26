#ifndef _LIST_H_
#define _LIST_H_

typedef int Element;
typedef struct node {
	Element data;
	struct node *next;
}Node;

typedef struct head{
	Node *frist;
	Node *last;
	int num;
}List;

#endif 