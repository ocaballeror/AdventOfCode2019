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
list_t* painted;

/**
 * Compare 2 coord_t structs. Return -1 if the first one is smaller, 1 if it's
 * bigger or 0 if the're equal
 * */
int coord_cmp(coord_t* one, coord_t* other) {
	if(one->x < other->x) {
		return -1;
	} else if (one->x > other->x) {
		return 1;
	} else {
		if(one->y < other->y) return -1;
		else if(one->y > other->y) return 1;
		else return 0;
	}
}

/**
 * Insert an element in a sorted list if it doesn't already exist.
 * */
void binary_insert(list_t* list, coord_t* coord) {
	int cut = 0, comp = 0;
	int start = 0, end = list->length - 1;
	while (start <= end) {
		cut = start + (end - start) / 2;
		coord_t* val = at_p(list, cut);

		comp = coord_cmp(coord, val);
		if(comp == 0) {
			// the element is already in the list. do nothing.
			free(coord);
			return;
		}
		if(comp == 1) {
			cut++;
			start = cut;
		} else {
			end = cut - 1;
		}
	}

	// insert the value at cut
	/* printf("Inserting at pos: %d\n", cut); */
	list_t* buffer = init_list();

	// remove all elements after the cut point
	while(list->length > cut){
		append_p(buffer, pop_p(list));
	}

	// insert the value
	append_p(list, coord);

	// restore all other elements
	while(buffer->length) {
		append_p(list, pop_p(buffer));
	}
	free_list(buffer);
}

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
		coord_t* copy = (coord_t*)malloc(sizeof(coord_t));
		copy->x = pos.x; copy->y = pos.y;
		binary_insert(painted, copy);
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


int main() {
	// Initialize global lists
	painted = init_list();
	map = init_dequeue();
	dequeue_t* row0 = init_dequeue();
	dequeue_append(row0, 0);
	dequeue_append_p(map, row0);

	// Monekypatch the input and output function with our new ones
	extern t_op operations[];
	operations[3].func = new_input;
	operations[4].func = new_output;

	t_memory* memory = read_input();
	start(memory);

	while(map->right->length){
		free_dequeue(shift_p(map->right));
	}
	while(map->left->length){
		free_dequeue(shift_p(map->left));
	}
	free_dequeue(map);

	printf("Panels painted: %d\n", painted->length);
	free_list(painted);

	free_t_memory(memory);
	return 0;
}
