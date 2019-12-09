#include <stdlib.h>

#ifndef INTCODE_H
#define INTCODE_H

typedef struct {
	long *registers;
	size_t regcount;
	int pc;
	int rel_base;
} t_memory;

typedef struct {
	int args;  // Number of args
	int output;  // Output arg number
	int branches;  // Whether or not it modifies the pc
	void (*func)(t_memory*, long*);
} t_op;

t_memory* init_t_memory(long *registers, size_t regcount);
void free_t_memory();

// Operations
long* read_input(size_t* size);
void noop(t_memory *memory, long *args);
void sum(t_memory *memory, long *args);
void mul(t_memory *memory, long *args);
void input(t_memory *memory, long *args);
void output(t_memory *memory, long *args);
void branch_true(t_memory *memory, long *args);
void branch_false(t_memory *memory, long *args);
void lessthan(t_memory *memory, long *args);
void equals(t_memory *memory, long *args);

void run_op(t_memory *memory);
void start(t_memory *memory);
long* run();

#endif
