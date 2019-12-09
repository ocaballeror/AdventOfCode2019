typedef struct {
	int *values;
	int length;
	int size;
	int shift;
} list_t;

list_t init_list();
int at(list_t*, int);
void print(list_t*, char*);
void append(list_t*, int);
void set(list_t*, int, int);
int shift(list_t*);
