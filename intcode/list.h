#ifndef LIST_H
#define LIST_H

typedef struct {
	void **values;
	int length;
	int size;
	int shift;
} list_t;

list_t* init_list();
void free_list();

void clear_list(list_t*);
void copy_list(list_t*, list_t*);

void* at_p(list_t*, int);
void append_p(list_t*, void*);
void set_p(list_t*, int, void*);
void* shift_p(list_t*);
void* pop_p(list_t*);

long at(list_t*, int);
void print(list_t*, char*);
void append(list_t*, long);
void set(list_t*, int, long);
long shift(list_t*);
long pop(list_t*);

#endif
