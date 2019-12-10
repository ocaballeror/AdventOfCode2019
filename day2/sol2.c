#include <stdio.h>
#include <stdlib.h>
#include "intcode.h"
#include "list.h"


int main(){
	t_memory *input = read_input();
	list_t* registers = malloc(sizeof(list_t));
	*registers = init_list();
	for(int i=0; i<input->registers->length; i++)
		append(registers, at(input->registers, i));
	t_memory* memory = init_t_memory(registers);

	int found = 0;
	int expect = 19690720;
	int noun, verb;
	for(noun=0; noun<=99 && !found; noun++){
		for(verb=0; verb<=99 && !found; verb++) {
			for(int i=0; i<input->registers->length; i++)
				set(registers, i, at(input->registers, i));
			set(registers, 1, noun);
			set(registers, 2, verb);
			start(memory);
			printf("n=%d, v=%d, res: %ld\n", noun, verb, at(registers, 0));
			if(at(registers, 0) == expect) {
				found = 1;
				noun--; verb--; // because the for loops will increment them before exiting
			}
		}
	}

	int solution = 100 * noun + verb;
	printf("Solution: %d\n", solution);

	free_t_memory(memory);
	free_t_memory(input);

	return 0;
}
