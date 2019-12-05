#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct {
	int args;  // Number of args
	int output;  // Output arg number
} t_op;

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
 * Read the program input.
 *
 * Takes a NULL int pointer to store the values that it reads from the file and
 * returns the number of elements in it.
 * */
int* read_input(size_t* size) {
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

// Run a single operation.
// Returns the new value for pc after execution
int run_op(int *registers, int pc) {
	int op = registers[pc];
	int opcode = op % 100;
	int n_args = operations[opcode].args;
	int output = operations[opcode].output;
	
	int* args = (int*)malloc(n_args * sizeof(int));
	for(int i=0; i < n_args; i++) {
		args[i] = registers[pc+i+1];
		int immediate = (int)(op / pow(10, i+2)) % 10;
		if(i+1 != output && !immediate) {
			args[i] = registers[args[i]];
		}
	}

	int input;
	int branched = 0;
	switch(opcode) {
		case 1:
			registers[args[2]] = args[0] + args[1];
			break;
		case 2:
			registers[args[2]] = args[0] * args[1];
			break;
		case 3:
			printf("Input: ");
			scanf("%d", &input);
			registers[args[0]] = input;
			break;
		case 4:
			printf("Out: %d\n", args[0]);
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

int run(int *registers) {
    // Run the program
    int pc = 0;
    while(registers[pc] != 99) {
		pc = run_op(registers, pc);
    }

    return registers[0];
}

int main(){
    size_t count;
    int *registers = read_input(&count);
	/* int registers[] = {101,-1,7,7,4,7,1105,11,0,99}; */
	run(registers);

    /* printf("Final state: "); */
    /* for(int i=0; i<count; i++) { */
    /*     printf("%d,", registers[i]); */
    /* } */
    /* printf("\n"); */

    free(registers);

    return 0;
}
