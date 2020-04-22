#include <stdio.h>
#include "intcode.h"
#include "list.h"

typedef struct {
	int x;
	int y;
	int type;
} tile_t;

tile_t* current;
int blocks = 0;
int outcount = 0;

/**
 * Replacement for the native output operation
 * */
void new_output(t_memory *memory, long *args) {
	long value = args[0];
	if(outcount == 0) {
		current = malloc(sizeof(tile_t));
		current->x = value;
	} else if(outcount == 1) {
		current->y = value;
	} else {
		current->type = value;
		if(value == 2) blocks++;
	}
	outcount = (outcount + 1) % 3;
}


int main() {
	// Monekypatch the input and output function with our new ones
	extern t_op operations[];
	operations[4].func = new_output;

	// run the actual program
	run(0);
	printf("Number of blocks: %d\n", blocks);
	return 0;
}
