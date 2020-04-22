#ifndef DEQUEUE_H
#define DEQUEUE_H
#include "list.h"

typedef struct {
	list_t* right;
	list_t* left;
} dequeue_t;

dequeue_t* init_dequeue();
void free_dequeue(dequeue_t* deq);
void* dequeue_at_p(dequeue_t* deq, int index);
void dequeue_set_p(dequeue_t* deq, int index, void* value);
void dequeue_append_p(dequeue_t* deq, void* value);
void dequeue_append_left_p(dequeue_t* deq, void* value);
long dequeue_at(dequeue_t* deq, int index);
void dequeue_set(dequeue_t* deq, int index, long value);
void dequeue_append(dequeue_t* deq, long value);
void dequeue_append_left(dequeue_t* deq, long value);

#endif
