#include <stdio.h>
#include "intcode.h"
#include "list.h"

void newoutput(t_memory *memory, long *args) {
    if (args[0] <= 127)
        printf("%c", (int)args[0]);
    else
        printf("%ld", args[0]);
}

void newinput(t_memory *memory, long *args){
	long in;
	scanf("%ld", &in);
	set(memory->registers, args[0], in);
}


int main(int argc, char **argv) {
    operations[3].func = newinput;
    operations[4].func = newoutput;
    run(0);

    return 0;
}
