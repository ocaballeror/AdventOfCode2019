#include <stdio.h>
#include <stdlib.h>
#include "../list.h"

#define assert_equal(a, b) if(a!=b) return 0
#define assert_le(a, b) if(a>b) return 0

int test_append(){
	list_t* list = init_list();
	assert_equal(list->length, 0);
	int elems = 1000;

	for(int i=0; i<elems; i++) {
		append(list, i);
	}
	assert_equal(list->length, elems);
	assert_le(elems, list->size);
	for(int i=0; i<elems; i++) {
		assert_equal(at(list, i), i);
	}

	free_list(list);
	return 1;
}

int test_shift() {
	list_t* list = init_list();
	int elems = 100;
	for(int i=0; i<elems; i++) {
		append(list, i);
	}
	assert_equal(at(list, 98), 98);
	assert_equal(at(list, 99), 99);

	int pop = shift(list);
	assert_equal(pop, 0);
	pop = shift(list);
	assert_equal(pop, 1);
	assert_equal(list->shift, 2);
	assert_equal(list->length, 98);

	assert_equal(at(list, 0), 2);
	assert_equal(at(list, 97), 99);
	assert_equal(at(list, 96), 98);

	append(list, 0);
	append(list, 1);
	assert_equal(at(list, 0), 2);
	assert_equal(at(list, 97), 99);
	assert_equal(at(list, 98), 0);
	assert_equal(at(list, 99), 1);
	assert_equal(list->length, 100);

	free_list(list);
	return 1;
}

int test_set() {
	list_t* list = init_list();

	append(list, 1);
	assert_equal(at(list, 0), 1);

	set(list, 0, 2);
	assert_equal(at(list, 0), 2);

	set(list, 1, 3);
	assert_equal(at(list, 0), 2);
	assert_equal(at(list, 1), 3);

	set(list, 3, 4);
	assert_equal(at(list, 1), 3);
	assert_equal(at(list, 2), 0);
	assert_equal(at(list, 3), 4);

	free_list(list);
	return 1;
}

int main() {
	int (*tests[])() = {
		test_append,
		test_shift,
		test_set,
		NULL
	};
	char* names[] = {
		"test_append",
		"test_shift",
		"test_set",
	};

	for(int i=0; tests[i] != NULL; i++) {
		int ret = tests[i]();
		printf("%s...", names[i]);
		if(ret) {
			printf("\033[1;32mOK");
		} else {
			printf("\033[1;31mFAIL");
		}
		printf("\033[0m\n");
	}
	return 0;
}
