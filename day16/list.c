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
    list->factor = 1;
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
    index = ((index + 1) / list->factor) % list->length;
	return list->values[index + list->shift];
}

/**
 * Appends a new element to the end of the list
 * */
void append_p(list_t *list, void* value) {
	if(list->length + list->shift == list->size) {
		int newsize = list->size * 3 / 2;
		list->values = realloc(list->values, newsize * sizeof(void*));
		list->size = newsize;
	}
	list->values[list->length + list->shift] = value;
	list->length++;
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

int at(list_t* list, int index) {
	return *((int*)at_p(list, index));
}
void append(list_t* list, int value) {
	int *ptr = malloc(sizeof(int));
	*ptr = value;
	append_p(list, ptr);
}

/**
 * Print the list in one line, prefixed by the given string
 * */
void print(list_t *list, char *name) {
	printf("%s: [", name);
	for(int i=0; i<list->length; i++) {
		printf("%d", at(list, i));
		if(i < list->length-1){
			printf(",");
		}
	}
	printf("]\n");
}
