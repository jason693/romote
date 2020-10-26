#ifndef _QUEUE_H_
#define _QUEUE_H_
#define N 640*480*2

typedef char Element;
typedef struct node{
	Element data[N];
	struct node *next;
}Node;

typedef struct queue{
	Node *head,*tail;
	int num; //队长
}Queue; 
Queue* create_queue(void);
int Enqueue(Queue* q,Element *n);
int del_queue(Queue* q,Element *n);
int Queue_IS_Empty(Queue* q);
int clear_queue(Queue* q);
Queue* destory_Queue(Queue* q);

#endif 


