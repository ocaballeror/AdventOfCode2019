#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

/**
 * Initializes and returns a new list
 * */
list_t* init_list() {
	list_t* list = malloc(sizeof(list_t));
	list->values = malloc(10 * sizeof(void*));
	list->size = 10;
	list->length = 0;
	list->shift = 0;
	return list;
}

/**
 * Empty a list
 * */
void clear_list(list_t* list) {
	for(int i=list->shift; i<list->shift + list->length; i++) {
		free(list->values[i]);
	}
	list->length = 0;
}

/**
 * Free a list object
 * */
void free_list(list_t* list) {
	clear_list(list);
	free(list->values);
	free(list);
}

/**
 * Retrieve the element at the specified index. This is preferred over using []
 * on .values
 * */
void* at_p(list_t *list, int index){
	return list->values[index + list->shift];
}

/**
 * Appends a new element to the end of the list
 * */
void append_p(list_t *list, void* value) {
	if(list->length + list->shift == list->size) {
		int newsize = list->size * 3 / 2;
		void* new_values = malloc(newsize * sizeof(void*));
		memcpy(new_values, list->values, list->size * sizeof(void*));
		free(list->values);
		list->values = new_values;
		list->size = newsize;
	}
	list->values[list->length + list->shift] = value;
	list->length++;
}

/**
 * Set the given element at the specified index.
 * */
void set_p(list_t *list, int index, void* value) {
	while(index >= list->length) append(list, 0);
	free(list->values[index + list->shift]);
	list->values[index + list->shift] = value;
}


/**
 * Remove and returnt the first element of the list
 * */
void* shift_p(list_t *list) {
	void* ret = at_p(list, 0);
	list->shift++; list->length--;
	return ret;
}

/**
 * Create a copy of the list.
 *
 * The 'other' parameter must be an initalized list that will be cleared
 * beforehand
 * */
void copy_list(list_t* list, list_t* other) {
	clear_list(other);
	for(int i=0; i<list->length; i++) {
		append(other, at(list, i));
	}
}


////////////////////////////////////////////////////
// CONVENIENCE METHODS FOR SIMPLE LISTS OF NUMBERS
////////////////////////////////////////////////////

long at(list_t* list, int index) {
	if(index >= list->length)
		return 0;
	return *((long*)at_p(list, index));
}
void append(list_t* list, long value) {
	long *ptr = malloc(sizeof(long));
	*ptr = value;
	append_p(list, ptr);
}
void set(list_t* list, int index, long value){
	long *ptr = malloc(sizeof(long));
	*ptr = value;
	set_p(list, index, ptr);
}
long shift(list_t* list) {
	long* ptr = (long*) shift_p(list);
	long ret = *ptr;
	free(ptr);
	return ret;
}

/**
 * Print the list in one line, prefixed by the given string
 * */
void print(list_t *list, char *name) {
	printf("%s: [", name);
	for(int i=0; i<list->length; i++) {
		printf("%ld", at(list, i));
		if(i < list->length-1){
			printf(",");
		}
	}
	printf("]\n");
}
