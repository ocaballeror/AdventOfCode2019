#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#include "intcode.h"


// Store the list of settings for the operations
t_op operations [] = {
	{0, 0,  0, noop}, // empty (operation 0 doesn't exist)
	{3, 3,  0, sum},
	{3, 3,  0, mul},
	{1, 1,  0, input},
	{1, -1, 0, output},
	{2, -1, 1, branch_true},
	{2, -1, 1, branch_false},
	{3, 3,  0, lessthan},
	{3, 3,  0, equals},
	{1, -1, 0, setbase},
};

t_memory* init_t_memory(list_t* registers) {
	t_memory *memory = malloc(sizeof(t_memory));
	memory->registers = registers;
	memory->pc = 0; memory->rel_base = 0;
	return memory;
}
void free_t_memory(t_memory* memory) {
	free_list(memory->registers);
	free(memory);
}

/**
 * Read the program input.
 *
 * Takes a NULL int pointer to store the values that it reads from the file and
 * returns the number of elements in it.
 * */
t_memory* read_input() {
	FILE *file = fopen("input", "r");
	if(!file) {
		perror("Input file");
		exit(1);
	}

	// Read our input and store it in the registers list
	list_t* registers = init_list();
	int i = 0;
	long read;
	while((fscanf(file, "%ld,", &read)) != EOF){
		append(registers, read);
		i++;
	}
	fclose(file);

	t_memory* memory = init_t_memory(registers);
	return memory;
}

void noop(t_memory *memory, long *args) {}
void sum(t_memory *memory, long *args) {
	set(memory->registers, args[2], args[0] + args[1]);
}
void mul(t_memory *memory, long *args) {
	set(memory->registers, args[2], args[0] * args[1]);
}
void input(t_memory *memory, long *args){
	long in;
	printf("Input: ");
	scanf("%ld", &in);
	set(memory->registers, args[0], in);
}
void output(t_memory *memory, long *args) {
	printf("Out: %ld\n", args[0]);
}
void branch_true(t_memory *memory, long *args) {
	if(args[0])
		memory->pc = args[1];
	else
		memory->pc += 3;  // Manually skip to the next instruction
}
void branch_false(t_memory *memory, long *args) {
	if(!args[0])
		memory->pc = args[1];
	else
		memory->pc += 3;  // Manually skip to the next instruction
}
void lessthan(t_memory *memory, long *args) {
	set(memory->registers, args[2], (args[0] < args[1]));
}
void equals(t_memory *memory, long *args) {
	set(memory->registers, args[2], (args[0] == args[1]));
}
void setbase(t_memory *memory, long *args) {
	/* printf("Set rel base to %d\n", args[0]); */
	memory->rel_base += args[0];
}

// Run a single operation.
void run_op(t_memory *memory) {
	list_t* registers = memory->registers;
	int op = at(registers, memory->pc);
	int opcode = op % 100;
	int n_args = operations[opcode].args;
	int output = operations[opcode].output;

	long* args = (long*)malloc(n_args * sizeof(long));
	for(int i=0; i < n_args; i++) {
		args[i] = at(registers, memory->pc+i+1);
		int mode = (int)(op / pow(10, i+2)) % 10;
		if(i+1 != output) {
			if(mode == 0){
				args[i] = at(registers, args[i]);
			} else if (mode == 2){
				args[i] = at(registers, memory->rel_base + args[i]);
			}
		} else {
			if(mode == 2) {
				args[i] += memory->rel_base;
			}
		}
	}

	/* printf("OP: %d, args: ", opcode); */
	/* for(int i=0; i<n_args; i++) printf("%ld ", args[i]); */
	/* printf("\n"); */

	operations[opcode].func(memory, args);
	if(!operations[opcode].branches){
		memory->pc += n_args + 1;
	}

	free(args);
}

void start(t_memory *memory) {
	// Run the program until it halts
	memory->pc = 0;
	while(at(memory->registers, memory->pc) != 99) {
		run_op(memory);
	}
}

long* run(int ret) {
	t_memory* memory = read_input();
	start(memory);
	if(ret) {
		long* copy = (long*)malloc(memory->registers->length * sizeof(long));
		memcpy(copy, memory->registers, memory->registers->length * sizeof(long));
		return copy;
	}
	free_t_memory(memory);
	return NULL;
}
