#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_AMPS 5

typedef struct {
	int args;  // Number of args
	int output;  // Output arg number
} t_op;

typedef struct {
	int *values;
	int length;
	int size;
	int shift;
} list_t;

/***************** LIST OPERATIONS ********************************/
/**
 * Initializes and returns a new list
 * */
list_t init_list() {
	int *initial = (int*)malloc(10 * sizeof(int));
	list_t list = {.size=10, .length=0, .shift=0, .values=initial};
	return list;
}

/**
 * Retrieve the element at the specified index. This is preferred over using []
 * on .values
 * */
int at(list_t *list, int index){
	return list->values[index + list->shift];
}

/**
 * Print the list in one line, prefixed by the given string
 * */
void print(list_t *list, char *name) {
	printf("%s: [", name);
	for(int i=0; i<list->length; i++) {
		printf("%d", at(list, i));
		if(i < list->length-1){
			printf(",");
		}
	}
	printf("]\n");
}

/**
 * Appends a new element to the end of the list
 * */
void append(list_t *list, int value) {
	if(list->length + list->shift == list->size) {
		int newsize = list->size * 3 / 2;
		int* new_values = (int*)malloc(newsize * sizeof(int));
		memcpy(new_values, list->values, list->size * sizeof(int));
		free(list->values);
		list->values = new_values;
		list->size = newsize;
	}
	list->values[list->length + list->shift] = value;
	list->length++;
}

/**
 * Set the given element at the specified index.
 * */
void set(list_t *list, int index, int value) {
	list->values[index + list->shift] = value;
}


/**
 * Remove and returnt the first element of the list
 * */
int shift(list_t *list) {
	int ret = at(list, 0);
	list->shift++; list->length--;
	return ret;
}
/***************** LIST OPERATIONS ********************************/


// Store the list of settings for the operations
t_op operations [] = {
	{0, 0}, // empty (operation 0 doesn't exist)
	{3, 3}, // add
	{3, 3}, // multiply
	{1, 1}, // input
	{1, -1}, // output
	{2, -1}, // jump-if-true
	{2, -1}, // jump-if-false
	{3, 3}, // less-than
	{3, 3}, // equals
};

/**
 * Read the program code.
 *
 * Takes a NULL int pointer to store the values that it reads from the file and
 * returns the number of elements in it.
 * */
int* read_code(size_t* size) {
	FILE *file = fopen("input", "r");
	if(!file) {
		perror("Input file");
		exit(1);
	}

	// Count the number of elements in the input. start at one because the last
	// element doesn't have a comma
	int count = 1;
	char c;
	while ((c = fgetc(file)) != EOF) {
		if (c == ',') count ++;
	}
	*size = count;

	// Read our input and store it in the registers array
	int* registers = (int*)malloc(count * sizeof(int));
	fseek(file, 0, SEEK_SET);
	int i = 0;
	while((fscanf(file, "%d,", &registers[i])) != EOF) i++;
	fclose(file);

	return registers;
}

void swap(int *x, int *y) {
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void permute(int *a, int l, int r, list_t *ret) {
	if (l == r) {
		int elem = 0;
		for(int i=5-1; i>=0; i--) {
			elem += a[i] * pow(10, i);
		}
		append(ret, elem);
	}
	else {
		for (int i = l; i <= r; i++) {
			swap((a+l), (a+i));
			permute(a, l+1, r, ret);
			swap((a+l), (a+i));
		}
	}
}

/**
 * If the input list has data, pop the first element from it. Otherwise ask the
 * user for input.
 * */
int get_input(list_t* input) {
	if(input->length > 0) {
		return shift(input);
	}
	int in;
	printf("Input: ");
	scanf("%d", &in);
	return in;
}

// Run a single operation.
// Returns the new value for pc after execution
int run_op(int* registers, int pc, list_t* input, list_t *output) {
	int op = registers[pc];
	int opcode = op % 100;
	int n_args = operations[opcode].args;
	int writes = operations[opcode].output;

	int* args = (int*)malloc(n_args * sizeof(int));
	for(int i=0; i < n_args; i++) {
		args[i] = registers[pc+i+1];
		int immediate = (int)(op / pow(10, i+2)) % 10;
		if(i+1 != writes && !immediate) {
			args[i] = registers[args[i]];
		}
	}

	int branched = 0;
	switch(opcode) {
		case 1:
			registers[args[2]] = args[0] + args[1];
			break;
		case 2:
			registers[args[2]] = args[0] * args[1];
			break;
		case 3:
			registers[args[0]] = get_input(input);
			break;
		case 4:
			append(output, args[0]);
			break;
		case 5:
			if(args[0]) {
				pc = args[1];
				branched = 1;
			}
			break;
		case 6:
			if(!args[0]){
				pc = args[1];
				branched = 1;
			}
			break;
		case 7:
			registers[args[2]] = (args[0] < args[1]);
			break;
		case 8:
			registers[args[2]] = (args[0] == args[1]);
			break;
	}
	free(args);

	if(!branched) {
		pc += n_args + 1;
	}
	return pc;
}

list_t run(int *registers, list_t* input) {
	// Run the program
	int pc = 0;
	list_t output = init_list();
	while(registers[pc] != 99) {
		pc = run_op(registers, pc, input, &output);
	}

	return output;
}

int test_sequence(int* registers, size_t n_registers, int* init_sequence) {
	int amplifier[n_registers];
	list_t input = init_list(), output = init_list();
	set(&output, 0, 0);
	for(int i=0; i<N_AMPS; i++) {
		append(&input, init_sequence[i]);
		append(&input, at(&output, 0));
		memcpy(amplifier, registers, n_registers*sizeof(int));
		free(output.values);
		output = run(amplifier, &input);
	}

	int ret = at(&output, 0);
	free(output.values);
	free(input.values);
	return ret;
}

int simulate(int *registers, size_t n_registers) {
	int res, best = 0;
	int values[] = {0,1,2,3,4};
	list_t permutations = init_list();
	permute(values, 0, N_AMPS-1, &permutations);
	for(int i=0; i<permutations.length; i++) {
		int test[N_AMPS];
		int number = at(&permutations, i);
		for(int j=0; j<N_AMPS; j++) {
			test[N_AMPS-j-1] = (int)(number / pow(10, j)) % 10;
		}
		res = test_sequence(registers, n_registers, test);
		if(res > best) {
			best = res;
			/* printf("New best: %d: ", best); */
			/* for(int i=0; i<5; i++) */
			/* 	printf("%d,", test[i]); */
			/* printf("\n"); */
		}
	}
	return best;
}

int main(){
	size_t count;
	int *registers = read_code(&count);

	int out = simulate(registers, count);
	printf("Output: %d\n", out);

	free(registers);
	return 0;
}
