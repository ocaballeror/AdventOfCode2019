#include <stdio.h>
#include <stdlib.h>
#include "intcode.h"
#include "list.h"

int main(){
	t_memory* memory = read_input();
	set(memory->registers, 1, 12);
	set(memory->registers, 2, 2);
	start(memory);
    printf("Solution: %ld\n", at(memory->registers, 0));
    free_t_memory(memory);
    return 0;
}
