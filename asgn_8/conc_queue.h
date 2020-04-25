#include <iostream>
#include <atomic>
#include <pthread.h>
#include<chrono>
#include<thread>

using namespace std;

extern int FULLSIZE;
struct node_t;
struct pointer_t;
struct queue_t;

typedef struct pointer_t{
	node_t* ptr;
	unsigned int count;
	unsigned int queueSize;

	bool operator ==(const pointer_t &other) const {
		return (ptr == other.ptr && count == other.count && queueSize == other.queueSize);
	}
}pointer_t;

typedef struct node_t{
	int value;
	atomic<pointer_t> next;
}node_t;

typedef struct queue_t{
	atomic<pointer_t> Head;
	atomic<pointer_t> Tail;
}queue_t;

void initialize(queue_t* Q);
bool enqueue(queue_t* Q, int val);
bool dequeue(queue_t* Q, int &pvalue);
