#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "intcode.h"
#include "list.h"

typedef struct {
	int x;
	int y;
	int type;
} tile_t;

long score = 0; // last score update we received
int outcount = 0; // track if we are on the first, second or third output instruction
tile_t* current_draw; // the tile we're currently drawing
tile_t* ball; // ball tile
tile_t* paddle; // paddle tile
list_t* tiles; // all the tiles in the map

// options
int draw = 1;
int play = 0;


/**
 * Sort tiles by y, then x. Ignore type.
 * */
int tile_cmp(const void* arg1, const void* arg2) {
	tile_t* one = (tile_t*)arg1;
	tile_t* other = (tile_t*)arg2;
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
 * Binary search for a tile in a list. Regular bsearch from stdlib doesn't work
 * here.
 *
 * Returns a pointer to the element if found, NULL otherwise.
 * */
tile_t* binary_search(list_t* list, tile_t* key) {
	int cut = 0, comp = 0;
	int start = 0, end = list->length - 1;
	while (start <= end) {
		cut = start + (end - start) / 2;
		tile_t* val = at_p(list, cut);

		comp = tile_cmp(key, val);
		if(comp == 0) {
			// found
			return val;
		}
		if(comp == 1) {
			start = cut + 1;
		} else {
			end = cut - 1;
		}
	}
	return NULL;
}



void print_map() {
	if(!draw) return;

	// clear the screen first
	printf("\033[2J\033[1;1H");

	tile_t* last = at_p(tiles, 0);
	for(int i=0; i<tiles->length; i++) {
		tile_t* tile = at_p(tiles, i);
		if(tile->y > last->y)
			printf("\n");
		switch(tile->type) {
			case 0: printf(" "); break;
			case 1:
				if(tile->y == 0){
					printf("x");
				} else {
					printf("|");
				}
				break;
			case 2: printf("#"); break;
			case 3: printf("_"); break;
			case 4: printf("·"); break;
		}
		last = tile;
	}
	printf("\nScore: %ld\n", score);
}

char read_char() {
	char c;
	struct termios new_kbd_mode;
	struct termios g_old_kbd_mode;

	/* put keyboard (stdin) in raw, unbuffered mode */
	tcgetattr (0, &g_old_kbd_mode);
	memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof (struct termios));

	new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
	new_kbd_mode.c_cc[VTIME] = 0;
	new_kbd_mode.c_cc[VMIN] = 1;
	tcsetattr (0, TCSANOW, &new_kbd_mode);

	scanf("%c", &c);

	/* set terminal back to regular mode */
	tcsetattr (0, TCSANOW, &g_old_kbd_mode);
	return c;
}


void new_input(t_memory* memory, long *args) {
	print_map();
	int move = 0;
	if(!play) {
		if(ball->x < paddle->x) {
			move = -1;
		} else if(ball->x > paddle->x) {
			move = 1;
		}
	} else {
		while(1) {
			char input = read_char();
			if(input == 'a') move = -1;
			else if(input == 's') move = 0;
			else if(input == 'd') move = 1;
			else continue;
			break;
		}
	}

	set(memory->registers, args[0], move);
}

/**
 * Update a tile on the tiles map. If a tile with the given coordinates does
 * not exist, append it
 *
 * Returns a pointer to the updated tile if found, NULL otherwise
 * */
tile_t* update_tile(tile_t* tile) {
	tile_t* pos = binary_search(tiles, tile);
	if(pos == NULL) {
		append_p(tiles, tile);
	} else {
		pos->type = tile->type;
	}
	return pos;
}

/**
 * Replacement for the native output operation
 * */
void new_output(t_memory *memory, long *args) {
	long value = args[0];
	if(outcount == 0) {
		current_draw = malloc(sizeof(tile_t));
		current_draw->x = value;
	} else if(outcount == 1) {
		current_draw->y = value;
	} else {
		if(current_draw->x == -1 && current_draw->y == 0){
			score = value;
			free(current_draw);
		} else {
			current_draw->type = value;
			tile_t* found = update_tile(current_draw);
			if(found != NULL) {
				free(current_draw);
				current_draw = found;
			}
			if(value == 3){
				paddle = current_draw;
			} else if(value == 4) {
				ball = current_draw;
			}
		}
	}
	outcount = (outcount + 1) % 3;
}


int main(int argc, char** argv) {
	// Initialize global lists
	tiles = init_list();

	// Monekypatch the input and output function with our new ones
	extern t_op operations[];
	operations[3].func = new_input;
	operations[4].func = new_output;

	// Parse arguments
	if(argc > 1) {
		for(int i=1; i<argc; i++){
			if(!strcmp(argv[i], "--play")){
				play = 1;
			} else if(!strcmp(argv[i], "--no-play")){
				play = 0;
			} else if(!strcmp(argv[i], "--draw")){
				draw = 1;
			} else if(!strcmp(argv[i], "--no-draw")){
				draw = 0;
			}
		}
	}

	t_memory* memory = read_input();
	// hack the arcade machine to play for free :D
	set(memory->registers, 0, 2);
	start(memory);

	// Print the final result
	if(draw)
		print_map();
	else 
		printf("Final score: %ld\n", score);

	free_t_memory(memory);
	free_list(tiles);
	return 0;
}
