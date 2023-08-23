#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


/* Función para manejar la redirección de entrada */
void handle_input_redirection(char *input_file)
{
	int fd = open(input_file, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening input file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting input");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

/* Función para manejar la redirección de salida */
void handle_output_redirection(char *output_file)
{
	int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting output");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void handle_double_output_redirection(char *output_file)
{
	int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting output");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

char *path_remover(char *arg)
{
	char *prog = NULL;
	char *arg_copy = _strdup(arg);
	char *token = strtok(arg_copy, "/");

	while (token != NULL)
	{
		token = strtok(NULL, "/");
		if (token)
		{
			free(prog);
			prog = _strdup(token);
		}
	}
	free(arg_copy);
	return prog;
}

/* Función para ajustar la sintaxis de redirección */
char *adjust_redirection_syntax(const char *input)
{
	char *adjusted = malloc(strlen(input) * 3 + 1); /* Espacio suficiente para la cadena ajustada */
	int adjusted_index = 0;
	int last_char_was_space = 0;

	for (int i = 0; input[i] != '\0'; i++)
	{
		if (input[i] == '<' || input[i] == '>' || (input[i] == '&' && input[i + 1] == '>'))
		{
			if (i > 0 && !last_char_was_space)
			{
				adjusted[adjusted_index++] = ' '; /* Añadir espacio antes del operador si no hay uno */
			}

			adjusted[adjusted_index++] = input[i];

			if (input[i + 1] != ' ')
			{
				adjusted[adjusted_index++] = ' '; /* Añadir espacio después del operador si no hay uno */
			}
			else
			{
				i++; /* Avanzar un carácter extra para evitar duplicar espacios */
			}

			last_char_was_space = 1; /* Establecer que el último carácter fue un espacio */
		}
		else
		{
			adjusted[adjusted_index++] = input[i];
			last_char_was_space = (input[i] == ' '); /* Actualizar si el último carácter fue un espacio */
		}
	}

	adjusted[adjusted_index] = '\0'; /* Terminar la cadena */

	return adjusted;
}


void execute_command(char *args[], int line_number)
{
	pid_t pid = 0;
	char *dir = NULL;
	char *path = NULL;
	char *path_copy = NULL;
	int saved_stdin, saved_stdout;

	/* Arreglo de variables de entorno para execve */
	char **env = environ;

	/* Verificar si hay redirección de entrada o salida */
	int input_redirect = 0;
	int output_redirect = 0;
	int double_output_redirect = 0;
	char *input_file = NULL;
	char *output_file = NULL;
	int i=0;

	/* Obtener el valor de los descriptores de archivo de entrada y salida estándar */
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	/* Obtener el valor de la variable de entorno PATH */
	path = _getenv("PATH");

	/* Duplicar la cadena de PATH para evitar modificaciones */
	path_copy = _strdup(path);

	/* Dividir la cadena PATH en directorios usando ":" como delimitador */
	dir = strtok(path_copy, ":");

	/* Crear un nuevo proceso hijo */
	pid = fork();

	/* Código dentro del proceso hijo */
	if (pid == 0) {

		for (i = 0; args[i] != NULL; i++)
		{
			if (_sstrcmp(args[i], "<") == 0)
			{
				input_redirect = 1;
				input_file = args[i + 1];
				args[i] = NULL;
			}
			else if (_sstrcmp(args[i], ">") == 0)
			{
				output_redirect = 1;
				output_file = args[i + 1];
				args[i] = NULL;
			}
			else if (_sstrcmp(args[i], ">>") == 0)
			{
				double_output_redirect = 1;
				output_file = args[i + 1];
				args[i] = NULL;
			}
		}

		/* Antes de ejecutar el comando, manejar la redirección de entrada y salida si es necesario */
		if (input_redirect) {
			handle_input_redirection(input_file);
		}

		if (output_redirect) {
			handle_output_redirection(output_file);
		}

		if (double_output_redirect) {
			handle_double_output_redirection(output_file);
		}

		char *adjusted_command = adjust_redirection_syntax(args[0]);
		printf("Comando ajustado: %s\n", adjusted_command);
		

		/* Verificar si el comando es ejecutable en la ubicación actual */
		if (access(adjusted_command, X_OK) == 0)
		{
			execve(adjusted_command, args, env);
			perror("Error executing command");
			exit(EXIT_FAILURE);
		}

		/* Recorrer los directorios en PATH */
		while (dir != NULL)
		{
			/* Almacenar la ruta completa del ejecutable */
			char executable_path[MAX_INPUT_LENGTH];
			snprintf(executable_path, sizeof(executable_path), "%s/%s", dir, adjusted_command);
			printf("Intentando ejecutar desde: %s\n", executable_path);


			/* Verificar si el comando es ejecutable en la nueva ruta */
			if (access(executable_path, X_OK) == 0)
			{
				/* Ejecutar el comando desde la nueva ruta */
				execve(executable_path, args, env);
			}

			/* Obtener el siguiente directorio en PATH */
			dir = strtok(NULL, ":");
		}

		/* Liberar la memoria de la copia de la cadena PATH */
		free(path_copy);

		/* Mostrar mensaje de comando no encontrado */
		fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, adjusted_command);
		/* Salir del proceso hijo con un código de error */
		exit(127);
	}
	else if (pid < 0)
	{
		print_error("Error forking");
	}
	else
	{
		free(path_copy);
		wait(NULL);

		/* Restaurar la entrada y salida estándar después de ejecutar el comando */
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
}
