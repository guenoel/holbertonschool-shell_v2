#include "shell.h"

/* Leer una línea de entrada desde el usuario */
char *read_input()
{
	char *line = NULL;
	size_t bufsize = MAX_INPUT_LENGTH;
	getline(&line, &bufsize, stdin); /* Leer la línea desde la entrada estándar */
	return (line); /* Devolver la línea leída */
}

/* Ejecutar el bucle principal de la shell */
void run_shell_loop(void)
{
	char *input;
	char *args[MAX_ARGS];
	int num_args;
	int line_number = 0;

	while (1) /* Bucle infinito para mantener la shell en funcionamiento */
	{
		line_number++; /* Incrementar el número de línea */
		if (isatty(STDIN_FILENO)) {
			printf("$ "); /* Mostrar el indicador de línea ($) solo en modo interactivo */
		}
		input = read_input(); /* Leer la línea de entrada */

		if (input == NULL)
		{
			printf("\n");
			break; /* Ctrl+D o EOF, salir del bucle */
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
			break; /* Salir de la shell si el comando es "exit" */
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
			execute_command(args, line_number); /* Ejecutar un comando externo */
		}

		if (!isatty(STDIN_FILENO))
		{
			break; /* Si no estamos en modo interactivo, salir del bucle */
		}

		free(input); /* Liberar la memoria de la línea de entrada */
		free_args(args); /* Liberar la memoria de los argumentos tokenizados */
	}
	free(input); /* Liberar la memoria de la última línea de entrada */
	free_args(args); /* Liberar la memoria de los últimos argumentos */
}

