#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


/* Función para manejar la redirección de entrada */
void handle_input_redirection(char *input_file, int line_number)
{
	int fd = open(input_file, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "./hsh: %d: cannot open %s: No such file\n", line_number, input_file);
		exit(2);
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

int execute_command(char *args[], int line_number)
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
	char executable_path[MAX_INPUT_LENGTH];
	int i = 0;
	int status;

	/* Obtener el valor de los descriptores de archivo de entrada y salida estándar */
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	/* Obtener el valor de la variable de entorno PATH */
	path = _getenv("PATH");

	/* Duplicar la cadena de PATH para evitar modificaciones */
	path_copy = _strdup(path);

	/* Dividir la cadena PATH en directorios usando ":" como delimitador */
	dir = strtok(path_copy, ":");

	/* Verificar si el comando es ejecutable en la ubicación actual */
	if (access(args[0], X_OK) == -1 || !_strchr((args[0]), '/'))
	{
		/* Recorrer los directorios en PATH */
		while (dir != NULL)
		{
			/* Almacenar la ruta completa del ejecutable */
			sprintf(executable_path, "%s/%s", dir, args[0]);

			/* Verificar si el comando es ejecutable en la nueva ruta */
			if (access(executable_path, X_OK) == 0)
			{
				break;
			}
			/* Obtener el siguiente directorio en PATH */
			dir = strtok(NULL, ":");
		}

		if (dir == NULL)
		{
			/* Mostrar mensaje de comando no encontrado */
			fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, args[0]);
			free(path_copy);
			/* Salir del proceso hijo con un código de error */
			return(127);
		}
	} else {
		_strcpy(executable_path, args[0]);
	}

	pid = fork();
	if (pid == 0)
	{
		for (i = 0; args[i] != NULL; i++)
		{
			if (_sstrcmp(args[i], "<") == 0)
			{
				input_redirect = 1;
				input_file = args[i + 1];
				args[i] = NULL;
			} else if (_sstrcmp(args[i], ">") == 0)
			{
				output_redirect = 1;
				output_file = args[i + 1];
				args[i] = NULL;
			} else if (_sstrcmp(args[i], ">>") == 0)
			{
				double_output_redirect = 1;
				output_file = args[i + 1];
				args[i] = NULL;
			}
		}

		/* Antes de ejecutar el comando, manejar la redirección de entrada y salida si es necesario */
		if (input_redirect) {
			int fd = open(input_file, O_RDONLY);
			if (fd == -1) {
				fprintf(stderr, "./hsh: %d: cannot open %s: No such file\n", line_number, input_file);
				free(path_copy);
				free(input_file);
				return (2);
			}
			close(fd);
		}

		if (output_redirect) {
			handle_output_redirection(output_file);
		}

		if (double_output_redirect) {
			handle_double_output_redirection(output_file);
		}
		free(path_copy);
		execve(executable_path, args, env);
		/* Mostrar mensaje de error si execve falla */
		perror("Error executing command");
		/* Salir del proceso hijo con un código de error */
		free(path_copy);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		print_error("Error forking");
	}
	else
	{
		free(path_copy);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			return(exit_status);
		} else
		{
			printf("Child process did not exit normally\n");
		}

		/* Restaurar la entrada y salida estándar después de ejecutar el comando */
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	return(0);
}
