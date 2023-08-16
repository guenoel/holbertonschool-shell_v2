#include "shell.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	char *args[MAX_ARGS]; /* Arreglo de punteros a caracteres para almacenar los tokens */
	/* printf("argc: %d\n", argc); */
	if (argc == 1)
	{
		/* Modo interactivo */
		run_shell_loop(); /* Llamar a la función para ejecutar el bucle de la shell */
	}
	else
	{
		/* printf("modo no interactivo main\n"); */
		/* Modo no interactivo */
		FILE *input_file = fopen(argv[1], "r"); /* Abrir el archivo de entrada en modo lectura */

		if (input_file)
		{
			char line[MAX_INPUT_LENGTH]; /* Almacenar una línea leída del archivo */

			while (fgets(line, sizeof(line), input_file)) /* Leer líneas del archivo */
			{
				printf("$ %s", line); /* Mostrar el indicador de línea y la línea leída */
				tokenize_input(line, args); /* Tokenizar la línea leída y almacenar los tokens en args */
			}

			fclose(input_file); /* Cerrar el archivo de entrada */
		}
		else
		{
			perror("Error opening input file"); /* Mostrar mensaje de error si no se puede abrir el archivo */
			return (1); /* Salir con código de error */
		}
	}

	return (0); /* Salir con éxito */
}
