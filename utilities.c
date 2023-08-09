#include "shell.h"
#include <stdio.h>

void print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}
