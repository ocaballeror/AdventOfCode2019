#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

int run(int *registers, int noun, int verb) {
    /*
     * Quoting from the website:
     * Before running the program, replace position 1 with the value 12 and
     * replace position 2 with the value 2
     * */
    registers[1] = noun;
    registers[2] = verb;

    // Run the program
    int pc = 0;
    while(registers[pc] != 99) {
        int first = registers[registers[pc + 1]];
        int second = registers[registers[pc + 2]];
        if(registers[pc] == 1) {
            registers[registers[pc + 3]] = first + second;
        } else if (registers[pc] == 2) {
            registers[registers[pc + 3]] = first * second;
        } else {
            printf("What is this: %d\n", registers[pc]);
        }
        pc += 4;
    }

    return registers[0];
}

int main(){
    size_t count;
    int *input = read_input(&count);
    int *registers = (int*)malloc(count * sizeof(int));
    memcpy(registers, input, count * sizeof(int));

    int res, found = 0;
    int expect = 19690720;
    int noun, verb;
    for(noun=0; noun<=99 && !found; noun++){
        for(verb=0; verb<=99 && !found; verb++) {
            memcpy(registers, input, count * sizeof(int));
            res = run(registers, noun, verb);
            // printf("n=%d, v=%d, res: %d\n", noun, verb, res);
            if(res == expect) {
                found = 1;
                noun--; verb--; // because the for loops will increment them before exiting
            }
        }
    }

    int solution = 100 * noun + verb;
    printf("Solution: %d\n", solution);

    free(input);
    free(registers);

    return 0;
}
