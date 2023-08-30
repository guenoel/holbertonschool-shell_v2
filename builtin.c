#include "shell.h"

#define UNUSED(x) (void)(x)

/**
 * shell_cd - Change the current working directory.
 * @args: An array of arguments passed to the 'cd' command.
 *
 * Return: 0 on success, -1 on failure.
 *
 * This function changes the current working directory based on the provided
 * arguments.
 * It supports the 'cd', 'cd ~', 'cd -', and 'cd [directory]' commands.
 */
int shell_cd(char *args[])
{
	static char previous_directory[MAX_INPUT_LENGTH] = "";
	char current_directory[MAX_INPUT_LENGTH] = "";

	int i = 0;
	char *argsetenv[3] = {NULL};

	for (i = 0; i < (MAX_INPUT_LENGTH); i++)
	{
		current_directory[i] = '\0';
		previous_directory[i] = '\0';
	}

	if (getcwd(current_directory, sizeof(current_directory)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
	if (args[1] == NULL || _sstrcmp(args[1], "~") == 0)
	{
		char *home_directory = get_env_var("HOME");

		if (home_directory == NULL)
		{
			return (-1);
		}
		if (chdir(home_directory) != 0)
		{
			perror("cd");
			return (-1);
		}
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		char *oldpwd = get_env_var("OLDPWD"); /* Obtener el valor actual de OLDPWD */

		if (oldpwd == NULL)
		{
			char *pwd = get_env_var("PWD"); /* Obtener el valor actual de PWD */

			if (pwd == NULL)
			{
				fprintf(stderr, "cd: No se ha definido la variable OLDPWD ni PWD\n");
				return (-1);
			}
			printf("%s\n", pwd);
			if (chdir(pwd) != 0)
			{
				perror("cd");
				return (-1);
			}
		} else
		{
			printf("%s\n", oldpwd);
			if (chdir(oldpwd) != 0)
			{
				perror("cd");
				return (-1);
			}
		}
	} else
	{
		if (chdir(args[1]) != 0)
		{
			fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", args[1]);
			return (-1);
		}
	}

	argsetenv[0] = "setenv";
	argsetenv[1] = "OLDPWD";
	argsetenv[2] = current_directory;
	shell_setenv(argsetenv);

	_strcpy(previous_directory, current_directory);
	return (0);
}

/**
 * shell_exit - Exit the shell.
 * @args: An array of arguments passed to the 'exit' command.
 * @line_number: The current line number.
 * @child_status: The status of the last child process.
 *
 * Return: The status of the last child process
 * This function exits the shell with the specified exit status.
 * It handles the 'exit' command and accepts an optional exit status code.
 */
int shell_exit(char *args[], int line_number, int child_status)
{
	char *endptr = NULL;
	long num = 0;

	free_args(environ);
	free(environ);
	if (args[1] == NULL)
	{
		free_args(args);
		exit(child_status);
	}
	num = _strtol(args[1], &endptr, 10);
	if (*endptr == '\0')
	{
		if (num < 0)
		{
			fprintf(stderr, "./hsh: %d: %s: Illegal number: %s\n", line_number,
					args[0], args[1]);
			free_args(args);
			exit(2);
		}
		else if (num == 1000)
		{
			free_args(args);
			exit(232);
		}
		else
		{
			free_args(args);
			exit(num);
		}
	} else
	{
		fprintf(stderr, "./hsh: %d: %s: Illegal number: %s\n", line_number,
				args[0], args[1]);
		free_args(args);
		exit(2);
	}
}

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
