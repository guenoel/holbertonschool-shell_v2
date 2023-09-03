#include "shell.h"

/**
 * free_args - Free memory occupied by an array of strings.
 * @args: An array of strings.
 *
 * This function frees the memory allocated for each string in the given array.
 */
void free_args(char *args[])
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	{
		free(args[i]); /* Liberar la memoria de cada argumento */
	}
}

/**
 * malloc_environ - Copy the environ array into a new variable.
 *
 * This function duplicates the environment variables from the 'environ' array
 * into a new array.
 * It allocates memory and ensures that the new array is properly terminated
 * with a NULL pointer.
 */
void malloc_environ(void) /* AGREGE VOID AL INTERIOR */
{
	char **new_env;
	int num_vars = 0;
	int i = 0;

	/* Count the number of variables in environ */
	while (environ[num_vars] != NULL)
	{
		num_vars++;
	}
	/* Allocate memory for the new array of pointers */
	new_env = (char **)malloc((num_vars + 1) * sizeof(char *));
	if (new_env == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	/* Copy each string from environ to the new array using strdup */
	for (i = 0; i < num_vars; i++)
	{
		new_env[i] = _strdup(environ[i]);
		if (new_env[i] == NULL)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
	}
	/* Terminate the new array with a NULL pointer */
	new_env[num_vars] = NULL;
	environ = new_env;
}

/**
 * init_array_of_strings - Initialize an array of strings with NULL values.
 * @array_of_strings: The array of strings to initialize.
 * @size: The size of the array.
 *
 * This function sets all elements in the array to NULL to initialize it for
 * later use.
 * Return: the number of elements
 */
int init_array_of_strings(char **array_of_strings, int size)
{
	int i = 0;

	for (i = 0; i < size; i++)
	{
		array_of_strings[i] = NULL;
	}
	return (0);
}

/**
 * init_string - Initialize a string with NULL characters.
 * @string: The string to initialize.
 * @size: The size of the string.
 *
 * This function sets all characters in the string to NULL to initialize it for
 * later use.
 * Return: status of the initialization
 */
int init_string(char *string, int size)
{
	int i = 0;

	for (i = 0; i < size; i++)
	{
		string[i] = '\0';
	}
	return (0);
}
