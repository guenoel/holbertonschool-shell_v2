#include "shell.h"

/**
 * getoptions - function that store option letters
 * @argv: all arguments
 * @argc: number of command line arguments
 * Return: string with all option letters
 */

char *getoptions(int argc, char *argv[])
{
	int i;
	char *options = (char *)malloc(1024);

	if (options == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}

	options[0] = '\0';

	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			char *opt = argv[i] + 1;
			char *opts = options;
			strcat(opts, opt);
		}
	}

	return (options);
}

/**
 * read_input - Reads a line of input from the user.
 *
 * Return: A dynamically allocated string containing the user's input,
 * or NULL in case of an error or end of file.
 */
/* Leer una línea de entrada desde el usuario */
char *read_input()
{
	char *line = NULL;
	ssize_t read_size;
	size_t bufsize = MAX_INPUT_LENGTH;

	read_size = getline(&line, &bufsize, stdin);
	if (read_size == -1)
	{
		free(line);
		return (NULL); /* Error o final del archivo */
	}

	return (line);
}


/**
 * run_shell_loop - Executes the main loop of the shell.
 *
 * Return: The exit status of the shell program.
 */
int run_shell_loop(void)
{
	char *input = NULL;
	char *args[MAX_ARGS];
	int num_args = 0;
	int line_number = 0;
	int status = 0;

	while (1) /* Bucle infinito para mantener la shell en funcionamiento */
	{
		line_number++; /* Incrementar el número de línea */
		if (isatty(STDIN_FILENO)) {
			printf("$ "); /* Mostrar el indicador de línea ($) solo en modo interactivo */
		}
		input = read_input(); /* Leer la línea de entrada */

		if (input == NULL)
		{
			if (isatty(STDIN_FILENO))
			{
				printf("\n");
			}
			break; /* Error o final del archivo */
		}

		if (_sstrcmp(input, "\n") == 0) /* Comparar con una línea en blanco */
		{
			free(input);
			continue; /* Línea vacía, volver al inicio del bucle */
		}

		num_args = tokenize_input(input, args); /* Tokenizar la línea de entrada */

		if (num_args == 0)
		{
			free(input);
			free_args(args);
			continue; /* Línea vacía, volver al inicio del bucle */
		}

		if (_sstrcmp(args[0], "exit") == 0)
		{
			free(input);
			shell_exit(args, line_number, status);
		}
		else if (_sstrcmp(args[0], "cd") == 0)
		{
			shell_cd(args); /* Ejecutar el comando "cd" */
		}
		else if (_sstrcmp(args[0], "env") == 0)
		{
			shell_env(args); /* Ejecutar el comando "env" */
		}
		else if (_sstrcmp(args[0], "setenv") == 0)
		{
			shell_setenv(args); /* Ejecutar el comando "setenv" */
		}
		else if (_sstrcmp(args[0], "unsetenv") == 0)
		{
			shell_unsetenv(args); /* Ejecutar el comando "unsetenv" */
		}
		else
		{
			if (status == 127)
			{
				free(input);
				free_args(args);
				free_args(environ);
				free(environ);
				exit(0);
			}
			status = execute_command(args, line_number); /* Ejecutar un comando externo */

		}
		free(input); /* Liberar la memoria de la línea de entrada */
		free_args(args); /* Liberar la memoria de los argumentos tokenizados */
	}
	return (status);
}

