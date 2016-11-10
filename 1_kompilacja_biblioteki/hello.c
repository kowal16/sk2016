#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
	printf("hello world!\n");
	extern char** environ;
	char** var;
	for (var = environ; *var != NULL; ++var)
	{
		printf("%s\n",*var);
	}
	printf("HOME %s\n",getenv("HOME"));
	setenv("TEST", "test", 1);
	printf("TEST %s\n",getenv("TEST"));

	assert(1 == 0);

	return 0;
}

