#include <stdio.h>
#include "intcode.h"
#include "list.h"
#include "dequeue.h"

typedef struct {
	int x;
	int y;
} coord_t;

int direction = 0; // direction the robot is facing
coord_t pos = {.x = 0, .y = 0};
int first_out = 1; // track if the robot is outputting the first or the second number

dequeue_t* map;

/**
 * Replacement for the native input operation.
 * */
void new_input(t_memory *memory, long *args){
	if(pos.y > 0 && map->right->length <= pos.y) {
		dequeue_append_p(map, init_dequeue());
	} else if(pos.y < 0 && map->left->length <= -pos.y) {
		dequeue_append_left_p(map, init_dequeue());
	}
	dequeue_t* row = dequeue_at_p(map, pos.y);
	while(1) {
		if(pos.x > 0 && row->right->length <= pos.x) {
			dequeue_append(row, 0);
		} else if(pos.x < 0 && row->left->length <= pos.x) {
			dequeue_append_left(row, 0);
		} else {
			break;
		}
	}

	int current = dequeue_at(row, pos.x);
	/* printf("(%d, %d) is %s\n", pos.x, pos.y, current? "white":"black"); */
	set(memory->registers, args[0], current);
}

/**
 * Replacement for the native output operation
 * */
void new_output(t_memory *memory, long *args) {
	long value = args[0];
	if(first_out) {
		// paint the current block
		dequeue_t* row = dequeue_at_p(map, pos.y);
		dequeue_set(row, pos.x, value);
		/* printf("Paint (%d, %d) %s\n", pos.x, pos.y, value? "white":"black"); */
	} else {
		// turn
		direction = (direction + (args[0]? 1:-1)) % 4;
		if(direction == -1) direction = 3;
		/* printf("Turned %d\n", direction); */

		// move
		switch(direction) {
			case 0: pos.y--; break;
			case 1: pos.x++; break;
			case 2: pos.y++; break;
			case 3: pos.x--; break;
		}
		/* printf("Moved to (%d, %d)\n", pos.x, pos.y); */
	}
	first_out = !first_out;
}

void paint_map() {
	int minx=0, maxx=0;
	int maxy = map->right->length, miny = map->left->length;
	for(int i=-miny; i<maxy; i++) {
		dequeue_t* row = dequeue_at_p(map, i);
		int min = row->left->length;
		int max = row->right->length;
		if(min < minx) minx = min;
		if(max > maxx) maxx = max;
	}

	for(int i=-miny; i<maxy; i++) {
		dequeue_t* row = dequeue_at_p(map, i);
		for(int i=-minx; i<maxx; i++) {
			if(dequeue_at(row, i) == 0)
				printf(".");
			else
				printf("#");
		}
		printf("\n");
	}
}


int main() {
	// Initialize global lists
	map = init_dequeue();
	dequeue_t* row0 = init_dequeue();
	dequeue_append(row0, 1);
	dequeue_append_p(map, row0);

	// Monekypatch the input and output function with our new ones
	extern t_op operations[];
	operations[3].func = new_input;
	operations[4].func = new_output;

	t_memory* memory = read_input();
	start(memory);
	paint_map();

	while(map->right->length){
		free_dequeue(shift_p(map->right));
	}
	while(map->left->length){
		free_dequeue(shift_p(map->left));
	}
	free_dequeue(map);

	free_t_memory(memory);
	return 0;
}
