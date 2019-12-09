#include <stdio.h>
#include <stdlib.h>
#include "intcode.h"

int main(){
	t_memory* memory = read_input();
	memory->registers[1] = 12;
	memory->registers[2] = 2;
	start(memory);
    printf("Solution: %ld\n", memory->registers[0]);
    free_t_memory(memory);
    return 0;
}
