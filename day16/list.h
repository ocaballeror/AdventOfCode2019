#ifndef LIST_H
#define LIST_H

typedef struct {
	void **values;
	int length;
	int size;
	int shift;
    int factor;
} list_t;

list_t* init_list();
void free_list();

void clear_list(list_t*);
void copy_list(list_t*, list_t*);

void* at_p(list_t*, int);
void append_p(list_t*, void*);

int at(list_t*, int);
void print(list_t*, char*);
void append(list_t*, int);

#endif
