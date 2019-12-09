#ifndef LIST_H
#define LIST_H

typedef struct {
	long *values;
	int length;
	int size;
	int shift;
} list_t;

list_t init_list();
long at(list_t*, int);
void print(list_t*, char*);
void append(list_t*, long);
void set(list_t*, int, long);
long shift(list_t*);

#endif
