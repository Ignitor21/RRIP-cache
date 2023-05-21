#pragma once

typedef struct QNode {
	long page;
	struct QNode *prev, *next;
} QNode;

typedef struct Queue {
	QNode *top, *end;
	long max_cell;
	long full_cell;
} Queue;

Queue *createQueue(long m);

int isQueueEmpty(Queue * queue);

void add_qnode(Queue * queue, long page, QNode ** hash);

int isQueueFull(Queue * queue);

int del_qnode(Queue * queue, QNode ** hash);

int lru(long page, Queue * queue, QNode ** hash);

void delete_hashLRU(QNode** hash_LRU);

void delete_queue(Queue * queue);
