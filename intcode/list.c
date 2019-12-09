#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

/**
 * Initializes and returns a new list
 * */
list_t init_list() {
	int *initial = (int*)malloc(10 * sizeof(int));
	list_t list = {.size=10, .length=0, .shift=0, .values=initial};
	return list;
}

/**
 * Retrieve the element at the specified index. This is preferred over using []
 * on .values
 * */
int at(list_t *list, int index){
	return list->values[index + list->shift];
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

/**
 * Appends a new element to the end of the list
 * */
void append(list_t *list, int value) {
	if(list->length + list->shift == list->size) {
		int newsize = list->size * 3 / 2;
		int* new_values = (int*)malloc(newsize * sizeof(int));
		memcpy(new_values, list->values, list->size * sizeof(int));
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
void set(list_t *list, int index, int value) {
	if(index+1 > list->length){
		print(list, "Invalid write");
		exit(1);
	}
	list->values[index + list->shift] = value;
}


/**
 * Remove and returnt the first element of the list
 * */
int shift(list_t *list) {
	int ret = at(list, 0);
	list->shift++; list->length--;
	return ret;
}
