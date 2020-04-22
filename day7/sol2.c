#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "list.h"
#include "intcode.h"

#define N_AMPS 5

list_t* input_buffer;
list_t* output_buffer;

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
void new_input(t_memory *memory, long *args){
	/* print(input_buffer, "New input: "); */
	if(input_buffer->length > 0) {
		set(memory->registers, args[0], shift(input_buffer));
	} else {
		int in;
		printf("Input: ");
		scanf("%d", &in);
	}
}

void new_output(t_memory *memory, long *args) {
	append(output_buffer, args[0]);
}

int test_sequence(t_memory* memory, int* init_sequence) {
	t_memory* amplifiers[N_AMPS];
	int current_amp = 0;

	clear_list(input_buffer);
	clear_list(output_buffer);
	for(int i=0; i<N_AMPS; i++) {
		list_t* copy = init_list();
		copy_list(memory->registers, copy);
		amplifiers[i] = init_t_memory(copy);
		// the input list will have gaps for the output of the amps
		// for ex.: [5, 5, 6, 6, 7, 7, 8, 8, 9, 9];
		// we will replace the second number of each with the output of the previous amp
		append(input_buffer, init_sequence[i]);
		append(input_buffer, init_sequence[i]);
	}
	set(input_buffer, 1, 0);  // Set the initial input for amp A

	int last_output = 0;
	while(at(amplifiers[current_amp]->registers, amplifiers[current_amp]->pc) != 99) {
		run_op(amplifiers[current_amp]);
		if(output_buffer->length > 0) {
			// If we already have data in the input_buffer buffer means we are already
			// running the init sequence, therefore skip the first number and
			// insert it at the second position
			last_output = shift(output_buffer);
			if (input_buffer->length > 0)
				set(input_buffer, 1, last_output);
			else
				append(input_buffer, last_output);

			// Switch to the next amp
			current_amp = (current_amp+1) % N_AMPS;
			/* printf("Switched to amp: %d, PC: %d\n", current_amp, pcs[current_amp]); */
		}
	}

	for(int i=0; i<N_AMPS; i++) {
		free_t_memory(amplifiers[i]);
	}

	return last_output;
}

int simulate(t_memory* memory) {
	int res = 0, best = 0;
	int values[] = {5,6,7,8,9};
	list_t* permutations = init_list();
	permute(values, 0, N_AMPS-1, permutations);
	for(int i=0; i<permutations->length; i++) {
		int test[N_AMPS];
		int number = at(permutations, i);
		for(int j=0; j<N_AMPS; j++) {
			test[N_AMPS-j-1] = (int)(number / pow(10, j)) % 10;
		}
		/* printf("Test: "); */
		/* for(int j=0; j<N_AMPS; j++) printf("%d", test[j]); */
		/* printf("\n"); */
		res = test_sequence(memory, test);
		if(res > best) {
			best = res;
			/* printf("New best: %d: ", best); */
			/* for(int i=0; i<5; i++) */
			/* 	printf("%d,", test[i]); */
			/* printf("\n"); */
		}
	}
	free_list(permutations);
	return best;
}

int main(){
	t_memory* memory = read_input();

	// Monekypatch the input and output function with our new ones
	extern t_op operations[];
	operations[3].func = new_input;
	operations[4].func = new_output;

	// Initialize global lists
	output_buffer = init_list();
	input_buffer = init_list();

	int out = simulate(memory);
	printf("Output: %d\n", out);

	free_list(output_buffer);
	free_list(input_buffer);
	free_t_memory(memory);
	return 0;
}
