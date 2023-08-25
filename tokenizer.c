#include "shell.h"

int tokenize_input(char *input, char *args[])
{
	int i = 0;
	char *token = strtok(input, " \t\n"); /* Obtén el 1 token de cadena input */
	char *redir_token;

	while (token != NULL)
	{
		/* Verifica si el token contiene un signo de redirección */
		redir_token = _strchr(token, '>');
		if (redir_token != NULL)
		{
			/* Si contiene '>', verifica si es '>>' o solo '>', y divide */
			if (redir_token != token)
			{
				*redir_token = '\0'; /* Coloca '\0' en lugar de '>' */
				args[i] = _strdup(token);
				i++;
			}

			if (redir_token[1] == '>') /* Es '>>' */
			{
				args[i] = _strdup(">>");
				i++;
				redir_token += 2; /* Avanza el puntero después de '>>' */
			}
			else /* Es '>' */
			{
				args[i] = _strdup(">");
				i++;
				redir_token++; /* Avanza el puntero después de '>' */
			}

			if (*redir_token != '\0')
			{
				args[i] = _strdup(redir_token);
				i++;
			}
		}
		else
		{
			/* Duplica el token actual y almacénalo en el arreglo args */
			args[i] = _strdup(token);
			i++;
		}

		token = strtok(NULL, " \t\n"); /* Obtén el siguiente token */
	}

	args[i] = NULL; /* Marca el final del arreglo args con un puntero nulo */
	return i;
}
