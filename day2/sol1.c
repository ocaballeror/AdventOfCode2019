#include <stdio.h>
#include <stdlib.h>


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

void run(int *registers) {
    /*
     * Quoting from the website:
     * Before running the program, replace position 1 with the value 12 and
     * replace position 2 with the value 2
     * */
    registers[1] = 12;
    registers[2] = 2;

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
}

int main(){
    size_t count;
    int *registers = read_input(&count);
    run(registers);

    /* printf("Final state: ");
     * for(int i=0; i<count; i++) {
     *     printf("%d,", registers[i]);
     * }
     * printf("\n");
     * */

    printf("Solution: %d\n", registers[0]);

    free(registers);

    return 0;
}
