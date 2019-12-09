#include <stdlib.h>
#include "intcode.h"

int main(){
	long *registers = run();
	free(registers);
	return 0;
}
