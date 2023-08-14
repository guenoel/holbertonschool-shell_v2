#include "shell.h"

int tokenize_input(char *input, char *args[])
{
	int i = 0;
	char *token = strtok(input, " \t\n");
		while (token != NULL)
	{
		args[i] = strdup(token); /* Duplicar el token*/
		token = strtok(NULL, " \t\n");
		i++;
	}
	args[i] = NULL;
	free(token);
	return (i);
}
