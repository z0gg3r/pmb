#include "error.h"

void check_oom(void *p, char *str)
{
	if (!p) {
		perror(str);

		exit(EXIT_FAILURE);
	}
}
