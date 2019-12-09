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
};

t_memory* init_t_memory(long* registers) {
	t_memory *memory = malloc(sizeof(t_memory));
	memory->registers = registers;
	memory->pc = 0; memory->rel_base = 0;
	return memory;
}
void free_t_memory(t_memory* memory) {
	free(memory->registers);
	free(memory);
}

/**
 * Read the program input.
 *
 * Takes a NULL int pointer to store the values that it reads from the file and
 * returns the number of elements in it.
 * */
long* read_input(size_t* size) {
	FILE *file = fopen("input", "r");
	if(!file) {
		perror("Input file");
		exit(1);
	}

	// Count the number of elements in the input. start at one because the last
	// element doesn't have a comma
	int count = 1;
	char c;
	while ((c = fgetc(file)) != EOF) {
		if (c == ',') count ++;
	}
	*size = count;

	// Read our input and store it in the registers array
	long* registers = (long*)malloc(count * sizeof(long));
	fseek(file, 0, SEEK_SET);
	int i = 0;
	while((fscanf(file, "%ld,", &registers[i])) != EOF) i++;
	fclose(file);

	return registers;
}

void noop(t_memory *memory, long *args) {}
void sum(t_memory *memory, long *args) {
	memory->registers[args[2]] = args[0] + args[1];
}
void mul(t_memory *memory, long *args) {
	memory->registers[args[2]] = args[0] * args[1];
}
void input(t_memory *memory, long *args){
	long in;
	printf("Input: ");
	scanf("%ld", &in);
	memory->registers[args[0]] = in;
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
	memory->registers[args[2]] = (args[0] < args[1]);
}
void equals(t_memory *memory, long *args) {
	memory->registers[args[2]] = (args[0] == args[1]);
}

// Run a single operation.
void run_op(t_memory *memory) {
	long* registers = memory->registers;
	int op = registers[memory->pc];
	int opcode = op % 100;
	int n_args = operations[opcode].args;
	int output = operations[opcode].output;

	long* args = (long*)malloc(n_args * sizeof(long));
	for(int i=0; i < n_args; i++) {
		args[i] = registers[memory->pc+i+1];
		int immediate = (int)(op / pow(10, i+2)) % 10;
		if(i+1 != output && !immediate) {
			args[i] = registers[args[i]];
		}
	}

	operations[opcode].func(memory, args);
	if(!operations[opcode].branches){
		memory->pc += n_args + 1;
	}

	free(args);
}

void start(t_memory *memory) {
	// Run the program until it halts
	memory->pc = 0;
	while(memory->registers[memory->pc] != 99) {
		run_op(memory);
	}
}

long* run() {
	size_t count;
	long *registers = read_input(&count);
	t_memory* memory = init_t_memory(registers);
	start(memory);
	long* copy = (long*)malloc(count * sizeof(long));
	memcpy(copy, registers, count * sizeof(long));
	free_t_memory(memory);
	return copy;
}
