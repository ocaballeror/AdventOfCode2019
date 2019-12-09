#include <stdio.h>
#include <stdlib.h>
#include "intcode.h"

int main(){
	size_t count;
	long *registers = read_input(&count);
	t_memory* memory = init_t_memory(registers, count);
	registers[1] = 12;
	registers[2] = 2;
	start(memory);
    printf("Solution: %ld\n", registers[0]);
    free_t_memory(memory);
    return 0;
}
