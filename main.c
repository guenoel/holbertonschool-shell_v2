#include "shell.h"

/**
 * main - Entry point for the shell program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 *
 * Return: The exit status of the shell program.
 */
int main(int argc, char *argv[])
{
	char *args[MAX_ARGS]; /* Arreglo punteros a caracteres almacenar los tokens */
	int status = 0;

	malloc_environ();

	if (argc == 1)
	{
		/* Modo interactivo */
		status = run_shell_loop(); /* Llamar a función ejecutar bucle de shell */
	}
	else
	{
		/* Modo no interactivo */
		FILE *input_file = fopen(argv[1], "r"); /* Abrir el archivo de entrada*/

		if (input_file)
		{
			char line[MAX_INPUT_LENGTH]; /* Almacenar una línea leída del archivo */

			while (fgets(line, sizeof(line), input_file)) /* Leer líneas del archivo */
			{
				printf("$ %s", line); /* indicador de línea y la línea leída */
				/* Tokenizar línea leída y almacenar los tokens en args */
				tokenize_input(line, args);
			}
			fclose(input_file); /* Cerrar el archivo de entrada */
		}
		else
		{
			perror("Error opening input file");
			return (1);
		}
	}
	free_args(environ);
	free(environ);
	return (status); /* Salir con éxito */
}
