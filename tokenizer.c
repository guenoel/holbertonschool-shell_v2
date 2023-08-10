#include "shell.h"

int tokenize_input(char *input, char *args[])
{
	int i = 0;
	char *token = NULL;
	
	token = strtok(input, " \t\n");
	/*printf("DEBUG: %p\n", token);*/
	while (token != NULL)
	{
		args[i] = _strdup(token); /* Duplicar el token*/
		token = strtok(NULL, " \t\n");
		i++;
	}
	/*printf("DEBUG: i = %d\n", i);*/
	args[i] = NULL;
	return (i);
}
