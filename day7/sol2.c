#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "list.h"

#define N_AMPS 5

typedef struct {
	int args;  // Number of args
	int output;  // Output arg number
} t_op;


// Store the list of settings for the operations
t_op operations [] = {
	{0, 0}, // empty (operation 0 doesn't exist)
	{3, 3}, // add
	{3, 3}, // multiply
	{1, 1}, // input
	{1, -1}, // output
	{2, -1}, // jump-if-true
	{2, -1}, // jump-if-false
	{3, 3}, // less-than
	{3, 3}, // equals
};

/**
 * Read the program code.
 *
 * Takes a NULL int pointer to store the values that it reads from the file and
 * returns the number of elements in it.
 * */
int* read_code(size_t* size) {
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
	int* registers = (int*)malloc(count * sizeof(int));
	fseek(file, 0, SEEK_SET);
	int i = 0;
	while((fscanf(file, "%d,", &registers[i])) != EOF) i++;
	fclose(file);

	return registers;
}

void swap(int *x, int *y) {
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void permute(int *a, int l, int r, list_t *ret) {
	if (l == r) {
		int elem = 0;
		for(int i=5-1; i>=0; i--) {
			elem += a[i] * pow(10, i);
		}
		append(ret, elem);
	}
	else {
		for (int i = l; i <= r; i++) {
			swap((a+l), (a+i));
			permute(a, l+1, r, ret);
			swap((a+l), (a+i));
		}
	}
}

/**
 * If the input list has data, pop the first element from it. Otherwise ask the
 * user for input.
 * */
int get_input(list_t* input) {
	if(input->length > 0) {
		return shift(input);
	}
	int in;
	printf("Input: ");
	scanf("%d", &in);
	return in;
}

// Run a single operation.
// Returns the new value for pc after execution
int run_op(int* registers, int pc, list_t* input, list_t *output) {
	int op = registers[pc];
	int opcode = op % 100;
	int n_args = operations[opcode].args;
	int writes = operations[opcode].output;

	int* args = (int*)malloc(n_args * sizeof(int));
	for(int i=0; i < n_args; i++) {
		args[i] = registers[pc+i+1];
		int immediate = (int)(op / pow(10, i+2)) % 10;
		if(i+1 != writes && !immediate) {
			args[i] = registers[args[i]];
		}
	}

	int branched = 0;
	switch(opcode) {
		case 1:
			registers[args[2]] = args[0] + args[1];
			break;
		case 2:
			registers[args[2]] = args[0] * args[1];
			break;
		case 3:
			registers[args[0]] = get_input(input);
			break;
		case 4:
			append(output, args[0]);
			break;
		case 5:
			if(args[0]) {
				pc = args[1];
				branched = 1;
			}
			break;
		case 6:
			if(!args[0]){
				pc = args[1];
				branched = 1;
			}
			break;
		case 7:
			registers[args[2]] = (args[0] < args[1]);
			break;
		case 8:
			registers[args[2]] = (args[0] == args[1]);
			break;
	}
	free(args);

	if(!branched) {
		pc += n_args + 1;
	}
	return pc;
}

int test_sequence(int* registers, size_t n_registers, int* init_sequence) {
	int amplifiers[N_AMPS][n_registers];
	int pcs[N_AMPS] = {0};
	int current_amp = 0;
	list_t input = init_list();
	list_t output = init_list();
	for(int i=0; i<N_AMPS; i++) {
		memcpy(amplifiers[i], registers, n_registers*sizeof(int));
		// the input list will have gaps for the output of the amps
		// for ex.: [5, 5, 6, 6, 7, 7, 8, 8, 9, 9];
		// we will replace the second number of each with the output of the previous amp
		append(&input, init_sequence[i]);
		append(&input, init_sequence[i]);
	}
	set(&input, 1, 0);  // Set the initial input for amp A

	int last_output = 0;
	while(amplifiers[current_amp][pcs[current_amp]] != 99) {
		pcs[current_amp] = run_op(amplifiers[current_amp], pcs[current_amp], &input, &output);
		if(output.length > 0) {
			// If we already have data in the input buffer means we are already
			// running the init sequence, therefore skip the first number and
			// insert it at the second position
			/* int insert_idx = input.length > 0? 1 : 0; */
			last_output = shift(&output);
			if (input.length > 0)
				set(&input, 1, last_output);
			else
				append(&input, last_output);

			// Switch to the next amp
			current_amp = (current_amp+1) % N_AMPS;
			/* printf("Switched to amp: %d, PC: %d\n", current_amp, pcs[current_amp]); */
		}
	}

	free(output.values);
	free(input.values);
	return last_output;
}

int simulate(int *registers, size_t n_registers) {
	int res = 0, best = 0;
	int values[] = {5,6,7,8,9};
	list_t permutations = init_list();
	permute(values, 0, N_AMPS-1, &permutations);
	for(int i=0; i<permutations.length; i++) {
		int test[N_AMPS];
		int number = at(&permutations, i);
		for(int j=0; j<N_AMPS; j++) {
			test[N_AMPS-j-1] = (int)(number / pow(10, j)) % 10;
		}
		res = test_sequence(registers, n_registers, test);
		if(res > best) {
			best = res;
			/* printf("New best: %d: ", best); */
			/* for(int i=0; i<5; i++) */
			/* 	printf("%d,", test[i]); */
			/* printf("\n"); */
		}
	}
	free(permutations.values);
	return best;
}

int main(){
	size_t count;
	int *registers = read_code(&count);

	int out = simulate(registers, count);
	printf("Output: %d\n", out);

	free(registers);
	return 0;
}
