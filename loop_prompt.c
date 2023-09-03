#include "shell.h"
/**
 * run_shell_loop - Executes the main loop of the shell.
 *
 * Return: The exit status of the shell program.
 */
int run_shell_loop(void)
{
	char *line = NULL;
	char *commands[MAX_ARGS] = {NULL};
	int line_number = 0, status = 0, logic_flag = 0;

	while (1) /* Bucle infinito para mantener la shell en funcionamiento */
	{
		line_number++; /* Incrementar el número de línea */
		logic_flag = 0;

		if (isatty(STDIN_FILENO))
			printf("$ ");	 /* indicador de línea ($) solo en modo interactivo */
		line = read_input(); /* Leer la línea de entrada */
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break; /* Error o final del archivo */
		}
		if (_sstrcmp(line, "\n") == 0) /* Comparar con una línea en blanco */
		{
			free(line);
			continue; /* Línea vacía, volver al inicio del bucle */
		}
		logic_flag = tokenize_line(commands, line, &status);
		status = handle_logic(commands, logic_flag, line, line_number, status);
		free(line);
	}
	return (status);
}
