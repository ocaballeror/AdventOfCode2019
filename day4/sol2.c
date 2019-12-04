#include <stdio.h>
#include <stdlib.h>

#define LENGTH 6

int valid(int number) {
    int duplicated = 0;
    char last = '0', secondlast = '0';
    char string[LENGTH + 1]; 
    sprintf(string, "%d", number);
    for(int i=0; i<LENGTH; i++) {
        if(string[i] < last) return 0;
        // Kind of complicated, but if we look at the two last ones and th
        // next, we can be sure that it's not part of a bigger group
        if(string[i] == last && string[i] != secondlast && (i == LENGTH -1 || string[i+1] != string[i])) {
            duplicated = 1;
        }
        secondlast = last;
        last = string[i];
    } 
    return duplicated;
}

int main(){
    char *input = "134564-585159";
    int start, end;
    sscanf(input, "%d-%d", &start, &end);

    int good = 0;
    for(int i=start; i<end; i++){
        good += valid(i); 
    }
    printf("Matches: %d\n", good);

	return 0;
}
