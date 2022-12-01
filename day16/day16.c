#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

char* read_input() {
    FILE* fp = fopen("input", "r");

    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* data = malloc(fsize);
    fread(data, sizeof(char), fsize, fp);
    data[fsize-1] = '\0';
    fclose(fp);

    return data;
}

int* parse_input(char* input) {
    size_t len = strlen(input);

    int* nums = malloc(len * sizeof(int));
    for (int i=0; i<len; i++){
        sscanf(input++, "%1d", &(nums[i]));
    }

    return nums;
}

int* simulate(char* input, int n_phases) {
    list_t* pattern = init_list();
    append(pattern, 0);
    append(pattern, 1);
    append(pattern, 0);
    append(pattern, -1);

    int* signal = parse_input(input);
    size_t signal_l = strlen(input);

    for (int phase=0; phase<n_phases; phase++){
        printf("Phase %d\n", phase);

        int* new_signal = malloc(signal_l * sizeof(int));
        for (int digit=0; digit<signal_l; digit++){
            int total = 0;
            for (int i=0; i<signal_l; i++){
                /* printf("%d*%d + ", signal[i], at(pattern, i)); */
                int eh = signal[i] * at(pattern, i);
                total += eh;
                /* printf("total += %d\n", eh); */
            }
            /* printf(" = %d\n", abs(total) % 10); */
            new_signal[digit] = abs(total) % 10;
            pattern->factor++;
        }

        free(signal);
        signal = new_signal;
        pattern->factor = 1;
    }

    free_list(pattern);
    return signal;
}

int main(int argc, char** argv) {
    char* input = read_input();
    int* signal = simulate(input, 100);
    printf("Part 1:");
    for(int i=0; i<8; i++) printf("%d", signal[i]);
    printf("\n");

    free(signal);
    free(input);

    return 0;
}
