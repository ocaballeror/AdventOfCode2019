#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int PATTERN[4] = {0,1,0,-1};

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
    int* buf;
    int* signal = parse_input(input);
    size_t signal_l = strlen(input);
    int* new_signal = malloc(signal_l * sizeof(int));

    for (int phase=0; phase<n_phases; phase++){
        /*printf("Phase %d\n", phase);*/

        for (int digit=0; digit<signal_l; digit++){
            int total = 0;
            for (int i=0; i<signal_l; i++){
                /* printf("%d*%d + ", signal[i], at(pattern, i)); */
                int eh = signal[i] * PATTERN[((i+1) / (digit + 1)) % 4];
                total += eh;
                /* printf("total += %d\n", eh); */
            }
            /* printf(" = %d\n", abs(total) % 10); */
            new_signal[digit] = abs(total) % 10;
        }

        buf = signal;
        signal = new_signal;
        new_signal = buf;
    }

    free(new_signal);
    return signal;
}

void sim2(int* signal, size_t signal_l, int* newsignal) {
    long res = 0;
    for (int i = 0; i < signal_l; i++) {
        res += signal[i];
    }

    newsignal[0] = res % 10;
    for (int i = 0; i < signal_l - 1; i++){
        res -= signal[i];
        newsignal[i + 1] = res % 10;
    }
}

void part1() {
    char* input = read_input();
    int* signal = simulate(input, 100);
    printf("Part 1: ");
    for(int i=0; i<7; i++) printf("%d", signal[i]);
    printf("\n");

    free(signal);
    free(input);
}

void part2() {
    char* input = read_input();
    char offset_str[7];
    strncpy(offset_str, input, 7);
    int offset = atoi(offset_str);

    int copies = 5000;
    size_t input_len = strlen(input);
    int* signal = malloc(input_len * copies * sizeof(int));
    int* nums = parse_input(input);
    for (int i = 0; i<copies; i++) {
        memcpy(&signal[input_len * i], nums, input_len * sizeof(int));
    }
    offset -= input_len * 5000;
    /*printf("Offset: %d\n", offset);*/

    int* buf;
    int* newsignal = malloc(input_len * copies * sizeof(int));
    for (int phase = 0; phase < 100; phase++) {
        sim2(signal, input_len * 5000, newsignal);
        buf = signal;
        signal = newsignal;
        newsignal = buf;
    }
    free(newsignal);

    printf("Part 2: ");
    for(int i=offset; i<offset + 8; i++) printf("%d", signal[i]);
    printf("\n");

    free(nums);
    free(input);
    free(signal);
}

int main(int argc, char** argv) {
    part1();
    part2();

    return 0;
}
