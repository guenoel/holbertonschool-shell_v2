#include "shell.h"

/**
* tokenize_output - Tokenizes an input string into an array of arguments,
* handling redirection.
* @args: An array to store the resulting arguments.
* @redir_token: string of redirection sign
* @token: string token from line command
*/
void tokenize_output(char *args[], char *redir_token, char *token)
{
	int i = 0;

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

/**
* tokenize_input - Tokenizes an input string into an array of arguments,
* handling redirection.
* @args: An array to store the resulting arguments.
* @redir_token: string of redirection sign
* @token: string token from line command
*/
void tokenize_input(char *args[], char *redir_token, char *token)
{
	int i = 0;

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

/**
* tokenize_double_output - Tokenizes an input string
* into an array of arguments,
* handling redirection.
* @args: An array to store the resulting arguments.
* @token: string token from line command
*/
void tokenize_double_output(char *args[], char *token)
{
	int i = 0;

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

/**
* tokenize_command - Tokenizes an input string into an array of arguments,
* handling redirection.
* @input: The input string to tokenize.
* @args: An array to store the resulting arguments.
*
* Return: The number of arguments parsed.
*
* This function splits the input string into individual tokens, separating
* them by spaces, tabs, and newlines.
* It also handles input and output redirection tokens ("<", ">", ">>", "<<"),
* splitting them as separate arguments.
* The resulting tokens are stored in the 'args' array, and the function returns
* the number of arguments parsed.
*/
int tokenize_command(char *input, char *args[])
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
			tokenize_output(args, redir_token, token);
		}
		else if (_sstrcmp(token, "<<") == 0)
		{
			tokenize_double_output(args, token);
		}
		else
		{
			/* Verifica si el token contiene un signo de redirección de entrada '<' */
			redir_token = _strchr(token, '<');
			if (redir_token != NULL)
			{
				tokenize_input(args, redir_token, token);
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
