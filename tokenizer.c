#include "shell.h"

int tokenize_input(char *input, char *args[])
{
	int i = 0;
	char *token = strtok(input, " \t\n"); /* Obtén primer token cadena input */
	char *redir_token;

	while (token != NULL)
	{
		/* Verifica el token contiene signo redirección de salida '>' o '>>' */
		redir_token = _strchr(token, '>');
		if (redir_token != NULL)
		{
			/* contiene '>', verifica si '>>' o solo '>', y divide secuencia */
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
		else if (_sstrcmp(token, "<<") == 0)
		{
			/* Si contiene '<<', es una redirección de heredoc */
			args[i] = _strdup("<<");
			i++;
			token = strtok(NULL, " \t\n"); /* Obtén el delimitador del heredoc */
			if (token != NULL)
			{
				args[i] = _strdup(token);
				i++;
			}
		}
		else
		{
			/* Verifica si el token contiene un signo de redirección de entrada '<' */
			redir_token = _strchr(token, '<');
			if (redir_token != NULL)
			{
				/* Si contiene '<', verifica si es solo '<', y divide en consecuencia */
				if (redir_token != token)
				{
					*redir_token = '\0'; /* Coloca '\0' en lugar de '<' */
					args[i] = _strdup(token);
					i++;
				}
				args[i] = _strdup("<");
				i++;
				redir_token++; /* Avanza el puntero después de '<' */
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
		}
		token = strtok(NULL, " \t\n"); /* Obtén el siguiente token */
	}
	args[i] = NULL; /* Marca el final del arreglo args con un puntero nulo */
	return (i);
}
