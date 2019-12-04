#include <stdio.h>
#include <stdlib.h>

#define LENGTH 6

int valid(int number) {
    int duplicated = 0;
    char last = '0';
    char string[LENGTH + 1]; 
    sprintf(string, "%d", number);
    for(int i=0; i<LENGTH; i++) {
        if(string[i] < last) return 0;
        if(string[i] == last) duplicated = 1;
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
