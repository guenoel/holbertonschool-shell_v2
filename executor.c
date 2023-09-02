#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>


/**
* handle_input_redirection - Handles input redirection by opening and
* redirecting input from a file.
* @input_file: The name of the input file.
* @line_number: The line number in the shell script where redirection is being
* @input: The input
* @args: The arguments
* @path_copy: The path
* handled.
*/
void handle_input_redirection(char *input_file, int line_number,char *args[], char *path_copy, char *input)
{
	int fd = open(input_file, O_RDONLY);

	if (fd == -1)
	{
		fprintf(stderr, "./hsh: %d: cannot open %s: No such file\n",
				line_number, input_file);
		free(path_copy);
		free(input);
		free(input_file);
		free_args(args);
		free_args(environ);
		free(environ);

		exit(2);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting input");
		exit(EXIT_FAILURE);
	}
	close(fd);
}


/**
* handle_output_redirection - Handles output redirection by opening and
* redirecting output to a file.
* @output_file: The name of the output file.
*/
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

/**
* handle_double_output_redirection - Handles double output redirection (append)
* by opening and redirecting output to a file in append mode.
* @output_file: The name of the output file for appending.
*/
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

/**
* handle_heredoc - Handles heredoc input redirection by reading lines until
* a delimiter is encountered and redirecting input from a pipe.
* @delimiter: The delimiter that marks the end of heredoc input.
*/
void handle_heredoc(char *delimiter)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int pipe_fd[2];
	/* off_t stdin_pos_before, stdin_pos_after; */
	/* Crear una tubería para redirigir las líneas al proceso hijo */
	if (pipe(pipe_fd) == -1)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) /* Proceso hijo */
	{
		close(pipe_fd[0]); /* Cerramos el extremo de lectura de la tubería */
		while ((read = getline(&line, &len, stdin)) != -1)
		{
			if (_strncmp(line, delimiter, _strlen(delimiter)) == 0)
			{
				break;
			}
			write(pipe_fd[1], line, read);
		}
		free_args(environ);
		free(environ);
		close(pipe_fd[1]); /* Cerramos el extremo de escritura de la tubería */
		exit(EXIT_SUCCESS);
	}
	else
	{
		/* Guardar la posición actual de stdin antes de la redirección */
		/* stdin_pos_before = lseek(STDIN_FILENO, 0, SEEK_CUR); */
		/* printf("Before redirection: %lld\n", (long long)stdin_pos_before); */

		close(pipe_fd[1]);
		/* Redirigir la entrada estándar desde la tubería */
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);

		lseek(STDIN_FILENO, 0, SEEK_END);

		/* Guardar la posición actual de stdin después de la redirección */
		/* stdin_pos_after = lseek(STDIN_FILENO, 0, SEEK_END); */
		/* printf("After redirection: %lld\n", (long long)stdin_pos_after); */

		free(line);
	}
}

/**
* execute_command - Executes command with optional input and output redirection
* @args: An array of strings representing the command and its arguments
* @line_number: Line number in the shell script where the command is executed
* @input: Command input
* Return: The exit status of the executed command
*/
int execute_command(char *args[], int line_number, char *input)
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

	int heredoc_redirect = 0;
	char *heredoc_delimiter = NULL;

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
			return (127);
		}
	}
	else
	{
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
				free(args[i]);
				args[i] = NULL;
				/* printf("Input redirection detected: %s\n", input_file); */
			}
			else if (_sstrcmp(args[i], ">") == 0)
			{
				output_redirect = 1;
				output_file = args[i + 1];
				free(args[i]);
				args[i] = NULL;
				/* printf("Output redirection detected: %s\n", output_file); */
			}
			else if (_sstrcmp(args[i], ">>") == 0)
			{
				double_output_redirect = 1;
				output_file = args[i + 1];
				free(args[i]);
				args[i] = NULL;
				/* printf("Double output redirection detected: %s\n", output_file); */
			}
			else if (_sstrcmp(args[i], "<<") == 0)
			{
				heredoc_redirect = 1;
				heredoc_delimiter = args[i + 1];
				free(args[i]);
				args[i] = NULL;
				/* printf("heredoc detected: %s\n", heredoc_delimiter); */
			}
			else
			{
				/* printf("Argumento args: %s\n", args[i]); */
			}
		}

		/* Antes de ejecutar el comando, manejar la redirección de entrada y salida si es necesario */
		if (input_redirect)
		{
			handle_input_redirection(input_file, line_number, args, path_copy, input);
			free(input_file);
		}

		if (output_redirect)
		{
			handle_output_redirection(output_file);
			free(output_file);
		}

		if (double_output_redirect)
		{
			handle_double_output_redirection(output_file);
			free(output_file);
		}

		if (heredoc_redirect)
		{
			handle_heredoc(heredoc_delimiter);
			free(heredoc_delimiter);

		}

		free(path_copy);
		execve(executable_path, args, env);
		/* Mostrar mensaje de error si execve falla */
		perror("Error executing command");
		/* Salir del proceso hijo con un código de error */

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
			return (exit_status);
		}
		else
		{
			printf("Child process did not exit normally\n");
		}

		/* Restaurar la entrada y salida estándar después de ejecutar el comando */
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	return (0);
}
