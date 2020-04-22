#include <stdlib.h>
#include "dequeue.h"

/**
 * Initialize and return a new dequeue
 * */
dequeue_t* init_dequeue() {
	dequeue_t* deq = malloc(sizeof(dequeue_t));
	deq->right = init_list();
	deq->left = init_list();
	return deq;
}


/**
 * Free a dequeue structure
 * */
void free_dequeue(dequeue_t* deq) {
	free_list(deq->right);
	free_list(deq->left);
	free(deq);
}

/**
 * Return the value at the given index
 * */
void* dequeue_at_p(dequeue_t* deq, int index) {
	if(index < 0) {
		return at_p(deq->left, -index-1);
	} else {
		return at_p(deq->right, index);
	}
}

/**
 * Replace the value at index with the given input
 * */
void dequeue_set_p(dequeue_t* deq, int index, void* value) {
	if(index < 0) {
		set_p(deq->left, -index-1, value);
	} else {
		set_p(deq->right, index, value);
	}
}

/**
 * Append a value to the right of the queue
 * */
void dequeue_append_p(dequeue_t* deq, void* value) {
	append_p(deq->right, value);
}

/**
 * Append a value to the left of the queue
 * */
void dequeue_append_left_p(dequeue_t* deq, void* value) {
	append_p(deq->left, value);
}

///////
// CONVENIENCE METHODS FOR SIMPLE LISTS OF NUMBERS
///////
long dequeue_at(dequeue_t* deq, int index) {
	if(index < 0){
		return at(deq->left, -index-1);
	} else {
		return at(deq->right, index);
	}
}

void dequeue_set(dequeue_t* deq, int index, long value) {
	if(index < 0){
		return set(deq->left, -index-1, value);
	} else {
		return set(deq->right, index, value);
	}
}

void dequeue_append(dequeue_t* deq, long value) {
	append(deq->right, value);
}

void dequeue_append_left(dequeue_t* deq, long value) {
	append(deq->left, value);
}
