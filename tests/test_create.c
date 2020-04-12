#include <stdio.h>

#include "avtka.h"

#define CHECK_EQUALS(x, y) do { if(x != y) return -1; } while (0)

int main()
{
	printf("EXPECTED PRINTS OF ERRORS BELOW, PLEASE RUN TESTS USING:\n"
	       " $ ninja test\n");
	struct avtka_t *a;
	a = avtka_create(0, 0);
	CHECK_EQUALS(a, 0);

	a = avtka_create("test", 0);
	CHECK_EQUALS(a, 0);

	struct avtka_opts_t opts = {0};
	a = avtka_create("test", &opts);
	CHECK_EQUALS(a, 0);

	opts.w = 360;
	opts.h = 240;
	a = avtka_create("test", &opts);
	CHECK_EQUALS(a > 0, 0);

	avtka_iterate(a);

	int32_t ret = avtka_destroy(a);
	CHECK_EQUALS(ret, 0);

	printf("EXPECTED PRINTS OF ERRORS BELOW, PLEASE RUN TESTS USING:\n"
	       " $ ninja test\n");
	return 0;
}
