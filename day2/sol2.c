#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intcode.h"


int main(){
    size_t count;
    long *input = read_input(&count);
    long* registers = (long*)malloc(count * sizeof(long));
	t_memory* memory = init_t_memory(registers, count);

    int found = 0;
    int expect = 19690720;
    int noun, verb;
    for(noun=0; noun<=99 && !found; noun++){
        for(verb=0; verb<=99 && !found; verb++) {
            memcpy(memory->registers, input, count * sizeof(long));
			registers[1] = noun;
			registers[2] = verb;
            start(memory);
            printf("n=%d, v=%d, res: %ld\n", noun, verb, registers[0]);
            if(registers[0] == expect) {
                found = 1;
                noun--; verb--; // because the for loops will increment them before exiting
            }
        }
    }

    int solution = 100 * noun + verb;
    printf("Solution: %d\n", solution);

    free(input);
	free_t_memory(memory);

    return 0;
}
