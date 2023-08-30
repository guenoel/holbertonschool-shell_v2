#include "shell.h"

/**
 * getoptions - function that store option letters
 * @argv: all arguments
 *
 * Return: string with all option letters
 */

char *getoptions(int argc, char *argv[])
{
	int i;
	char *options = (char *)malloc(1024);

	if (options == NULL) {
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

/* Leer una línea de entrada desde el usuario */
char *read_input()
{
	char *line = NULL;
	ssize_t read_size = 0;
	size_t bufsize = MAX_INPUT_LENGTH;

	read_size = getline(&line, &bufsize, stdin);

	if (read_size == -1)
	{
		free(line);
		return (NULL); /* Error o final del archivo */
	}

	return (line);
}


/* Ejecutar el bucle principal de la shell */
int run_shell_loop(void)
{
	char *input = NULL;
	char *args[MAX_ARGS];
	int num_args = 0;
	int line_number = 0;
	int status = 0;
	int is_heredoc = 0;
	char *ptr_found = NULL;
	char *ptr_found2 = NULL;
	char *delim = "random string";
	char *dup_input = NULL;
	char *cropped_input = NULL;
	int num_line_heredoc = 0;

	while (1) /* Bucle infinito para mantener la shell en funcionamiento */
	{
		line_number++; /* Incrementar el número de línea */
		if (isatty(STDIN_FILENO)) {
			printf("$ "); /* Mostrar el indicador de línea ($) solo en modo interactivo */
		}

		input = read_input(); /* Leer la línea de entrada */

		ptr_found = _strchr(input, '<');
		if (ptr_found != NULL)
			if (ptr_found[1] == '<')
			{
				num_line_heredoc = line_number;
				is_heredoc = 1;
			}
		if (input != NULL)
		{
			dup_input = _strdup(input);
			ptr_found2 = _strchr(dup_input, '\n');
			
			if (ptr_found2 != NULL)
			{
				cropped_input = strtok(dup_input, "\n");
			}
			if(cropped_input)
			{
				if (strcmp(cropped_input, delim) == 0)
				{
					is_heredoc = 0;
					/* free(delim); */
				}
			}
			free(dup_input);
		}

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

		if (is_heredoc == 1 && line_number == num_line_heredoc)
		{
			delim = _strdup(args[2]);
		}

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
			if ((is_heredoc == 0 || line_number == num_line_heredoc))
			{
				if(cropped_input)
				{
					if(_sstrcmp(cropped_input, delim) != 0)
					{
						status = execute_command(args, line_number); /* Ejecutar un comando externo */
					}
				}
			}

		}
		free(delim);
		free(input); /* Liberar la memoria de la línea de entrada */
		free_args(args); /* Liberar la memoria de los argumentos tokenizados */
	}
	return (status);
}
