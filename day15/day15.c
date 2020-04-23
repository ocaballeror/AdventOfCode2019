#include <assert.h>
#include <stdio.h>
#include "intcode.h"
#include "list.h"


typedef struct {
	int x;
	int y;
	int type;  // 0 wall, 1 traversable, 2 oxygen
	int dist;  // distance from (0, 0)
	list_t* path;  // path from (0, 0) to here
} coord_t;

int lastmove = 0;
list_t* map;
list_t* to_visit;
list_t* seen;

coord_t* currentpos;
list_t* current_path;
list_t* next_moves;

coord_t* first_oxygen;


/**
 * Return a string representation of a coordinate.
 *
 * This function leaks memory.
 * */
char* fmtcoord(coord_t* coord) {
	char* buffer = malloc(128 * sizeof(char));
	char type;
	if(coord->x == currentpos->x && coord->y == currentpos->y) {
		type = 'D';
	} else if(coord->type == 0) {
		type = '#';
	} else if(coord->type == 1) {
		type = '.';
	} else {
		type = 'O';
	}
	sprintf(buffer, "COORD (%d, %d), type: %c, dist: %d", coord->x, coord->y, type, coord->dist);
	return buffer;
}

void free_coord(coord_t* coord) {
	free_list(coord->path);
	free(coord);
}

void free_coordlist(list_t* list) {
	while(list->length) {
		free_coord(pop_p(list));
	}
	free_list(list);
}

void cleanup() {
	free_coord(currentpos);
	free_list(next_moves);
	free_list(current_path);
	free_coordlist(to_visit);
	free_coordlist(seen);
	free_coordlist(map);
}


/**
 * Returns the side from 1 to 4 that `other` in which adjacent to `one`.
 * Returns -1 if they aren't adjacent, 0 if they are the same;
 * */
int adjacent(coord_t* one, coord_t* other) {
	if(one->x == other->x && one->y == other->y) {
		return 0;
	} else if(one->x == other->x && one->y-1 == other->y) {
		return 1;
	} else if(one->x == other->x && one->y+1 == other->y) {
		return 2;
	} else if(one->y == other->y && one->x-1 == other->x) {
		return 3;
	} else if(one->y == other->y && one->x+1 == other->x) {
		return 4;
	} else {
		return -1;
	}
}

/**
 * Compare 2 coord_t structs. Return -1 if the first one is smaller, 1 if it's
 * bigger or 0 if the're equal
 * */
int coord_cmp(coord_t* one, coord_t* other) {
	if(one->y < other->y) {
		return -1;
	} else if (one->y > other->y) {
		return 1;
	} else {
		if(one->x < other->x) return -1;
		else if(one->x > other->x) return 1;
		else return 0;
	}
}

/**
 * Compare 2 coord_t structs by their distance.
 * */
int dist_cmp(coord_t* one, coord_t* other) {
	if(one->dist < other->dist){
		return -1;
	} else if(one->dist > other->dist){
		return 1;
	} else {
		// If they are at equal distance, prioritize the one closer to us
		if(adjacent(one, currentpos) != -1) {
			return -1;
		} else if(adjacent(other, currentpos) != -1) {
			return 1;
		}
		return coord_cmp(one, other);
	}
}

void print_map() {
	// clear the screen first
	printf("\033[2J\033[1;1H");

	int minx = 0, miny = 0;
	for(int i=0; i<map->length; i++) {
		coord_t* block = at_p(map, i);
		if(block->x < minx) minx = block->x;
		if(block->y < miny) miny = block->y;
	}

	int lasty = miny, lastx = minx;
	for(int i=0; i<map->length; i++) {
		coord_t* block = at_p(map, i);
		while(block->y > lasty) {
			lasty++;
			lastx = minx;
			printf("\n");
		}
		while(block->x > lastx+1) {
			lastx++;
			printf("-");
		}
		char type;
		if(block->x == currentpos->x && block->y == currentpos->y) {
			type = 'D';
		} else if(block->type == 0) {
			type = '#';
		} else if(block->type == 1) {
			type = '.';
		} else {
			type = 'O';
		}
		printf("%c", type);
		lastx = block->x; lasty = block->y;
	}
	printf("\n");
}


/**
 * Binary search for a coord in a sorted list.
 *
 * Returns the index of the coord if found, or the index where it should be
 * placed if it's not
 * */
int binary_search(list_t* list, coord_t* coord, int (*func)(coord_t*, coord_t*)) {
	int cut = 0, comp = 0;
	int start = 0, end = list->length - 1;
	while (start <= end) {
		cut = start + (end - start) / 2;
		coord_t* val = at_p(list, cut);

		comp = func(coord, val);
		if(comp == 0) {
			return cut;
		}
		if(comp == 1) {
			cut++;
			start = cut;
		} else {
			end = cut - 1;
		}
	}
	return cut;
}

/**
 * Insert an element in a sorted list if it doesn't already exist.
 * */
void binary_insert(list_t* list, coord_t* coord, int (*func)(coord_t*, coord_t*)) {
	int pos = binary_search(list, coord, func);
	if(pos != list->length && func(at_p(list, pos), coord) == 0){
		// element already in the list. do nothing
		free_coord(coord);
		return;
	}

	// insert the value at pos
	list_t* buffer = init_list();

	// remove all elements after the cut point
	while(list->length > pos){
		append_p(buffer, pop_p(list));
	}

	// insert the value
	append_p(list, coord);

	// restore all other elements
	while(buffer->length) {
		append_p(list, pop_p(buffer));
	}
	free_coordlist(buffer);
}

coord_t* find(list_t* list, coord_t* coord, int (*func)(coord_t*, coord_t*)) {
	if(list->length == 0){
		// empty list. no way we have that element here
		return NULL;
	}
	int pos = binary_search(list, coord, func);
	if(pos != list->length){
		coord_t* elem = at_p(list, pos);
		if (func(elem, coord) == 0) {
			return elem;
		}
	}
	return NULL;
}


/**
 * Return a pointer to a coord_t structure with the given parameters
 * */
coord_t* new_coord(int x, int y, int type, int dist) {
	coord_t* new = malloc(sizeof(coord_t));
	new->x = x; new->y = y; new->type = type; new->dist = dist;
	new->path = init_list();
	return new;
}

coord_t* copy_coord(coord_t* coord) {
	coord_t* ret = new_coord(coord->x, coord->y, coord->type, coord->dist);
	copy_list(coord->path, ret->path);
	return ret;
}

void move_coord(coord_t* coord, int direction) {
	switch(direction) {
		case 1: coord->y--; break;
		case 2: coord->y++; break;
		case 3: coord->x--; break;
		case 4: coord->x++; break;
	}
	coord->dist++;
	if(coord->x == 0 && coord->y == 0) {
		coord->dist = 0;
	}
}

int oposite(int move) {
	switch(move) {
		case 1: return 2;
		case 2: return 1;
		case 3: return 4;
		case 4: return 3;
		default: return 0;
	}
}

/**
 * Replacement for the native output operation.
 * */
void new_output(t_memory *memory, long *args) {
	long value = args[0];

	coord_t* new = copy_coord(currentpos);
	move_coord(new, lastmove);
	new->type = value;
	binary_insert(map, new, coord_cmp);

	if(value == 0){
		// We hit a wall
		if(current_path->length) {
			pop(current_path);
		}
	} else if(value == 1) {
		// Successful move
		move_coord(currentpos, lastmove);
	} else if(value == 2 && !first_oxygen) {
		// Found oxygen
		printf("Found the oxygen at (%d, %d), dist: %d\n", currentpos->x, currentpos->y, currentpos->dist);
		first_oxygen = copy_coord(currentpos);
	}
}

int move() {
	assert(next_moves->length);
	int move = shift(next_moves);
	if((currentpos->x == 0 && currentpos->y == 0) || current_path->length) {
		int last = at(current_path, current_path->length-1);
		if(oposite(last) == move) {
			// These moves would cancel each other out
			pop(current_path);
		}
		else {
			append(current_path, move);
		}
	}
	lastmove = move;
	return move;
}

void explore(coord_t* node) {
	// Dijkstra's algorithm
	for(int dir=1; dir<=4; dir++) {
		coord_t* move = copy_coord(node);
		append(move->path, dir);
		move_coord(move, dir);

		coord_t* mapblock = find(map, move, coord_cmp);
		if(mapblock) {
			// move is on the map
			if(mapblock->type == 0){
				// looking at a wall. ignore.
				free_coord(move);
				continue;
			}
			if(move->dist < mapblock->dist){
				// found a shorter distance to this block
				mapblock->dist = move->dist;
			}
		}
		if(!find(seen, move, coord_cmp)) {
			binary_insert(to_visit, copy_coord(move), dist_cmp);
		}
		free_coord(move);
	}
	binary_insert(seen, node, coord_cmp);
}

/**
 * Find the next move from the list of visitable nodes and put it in the next_moves list.
 *
 * Returns 0 when there are no more possible moves, 1 otherwise.
 * */
int nextmove() {
	if(!to_visit->length) {
		printf("No more moves\n");
		return 0;
	}

	coord_t* next = at_p(to_visit, 0);
	if(first_oxygen && first_oxygen->dist <= next->dist){
		// we already have a solution and the best to_visit has a longer
		// distance than that. halt.
		return 0;
	}

	// first check if he have already seen this block. we can't explore it
	// otherwise because it might be a wall
	coord_t* block = find(map, next, coord_cmp);
	if(block != NULL){
		next = shift_p(to_visit); // actually remove it from to_visit
		if(block->type != 0) {
			explore(next);
		} else {
			free_coord(next);
		}
	} else {
		int side = adjacent(currentpos, next);
		if(side != -1) {
			assert(next_moves->length == 0);
			append(next_moves, side);
		} else {
			// not adjacent. we'll have to trace a path back to (0, 0) and then
			// from that to our target
			while(current_path->length){
				int move = oposite(pop(current_path));
				append(next_moves, move);
			}
			for(int i=0; i<next->path->length; i++) {
				append(next_moves, at(next->path, i));
			}
		}
	}
	return 1;
}

/**
 * Replacement for the native input operation.
 *
 * Uses the global next_moves list to determine what the next input will be.
 * */
void new_input(t_memory *memory, long *args) {
	int what = 0;
	while(!next_moves->length) {
		if(nextmove() == 0) {
			// No more moves. Halt execution
			set(memory->registers, memory->pc + 2, 99);
			return;
		}
	}
	assert(next_moves->length);
	what = move();
	set(memory->registers, args[0], what);
}

void expand_oxygens() {
	assert(first_oxygen);
	list_t* oxygens = init_list();
	list_t* nextoxygens = init_list();
	append_p(oxygens, first_oxygen);
	int minutes = 0;

	while(oxygens->length) {
		while(oxygens->length) {
			coord_t* oxy = pop_p(oxygens);
			for(int dir=1; dir<=4; dir++) {
				move_coord(oxy, dir);
				coord_t* mapblock = find(map, oxy, coord_cmp);
				if(mapblock != NULL && mapblock->type == 1) {
					mapblock->type = 2;
					append_p(nextoxygens, copy_coord(oxy));
				}
				move_coord(oxy, oposite(dir));
			}
			free_coord(oxy);
		}
		if(nextoxygens->length) minutes++;
		list_t* tmp = oxygens;
		oxygens = nextoxygens;
		nextoxygens = tmp;
	}

	free_coordlist(oxygens);
	free_coordlist(nextoxygens);
	printf("Total minutes: %d\n", minutes);
}

/**
 * Initialize global lists
 * */
void setup() {
	map = init_list();
	coord_t* zero = new_coord(0, 0, 1, 0);
	append_p(map, zero);

	to_visit = init_list();
	append_p(to_visit, copy_coord(zero));
	seen = init_list();

	currentpos = copy_coord(zero);
	current_path = init_list();
	next_moves = init_list();
}

int main() {
	setup();

	// Monekypatch the input and output function with our new ones
	extern t_op operations[];
	operations[3].func = new_input;
	operations[4].func = new_output;

	// run the actual program
	run(0);

	// Part 2
	expand_oxygens();

	cleanup();
	return 0;
}
