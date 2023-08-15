#include "shell.h"

int tokenize_input(char *input, char *args[])
{
	int i = 0; /* Inicialización del contador de posiciones en el arreglo args */
	char *token = strtok(input, " \t\n"); /* Obtener el primer token de la cadena input */

	/* Ciclo para procesar todos los tokens */
	while (token != NULL)
	{
		/* Duplicar el token actual y almacenarlo en el arreglo args */
		args[i] = _strdup(token);
		token = strtok(NULL, " \t\n"); /* Obtener el siguiente token */
		i++; /* Incrementar el contador de posiciones */
	}

	args[i] = NULL; /* Marcar el final del arreglo args con un puntero nulo */
	return (i); /* Devolver la cantidad de tokens procesados */
}
