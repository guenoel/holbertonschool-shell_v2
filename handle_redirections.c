#include "shell.h"

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
void handle_input_redirection(char *input_file, int line_number, char *args[],
							char *path_copy, char *input)
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
				break;
			write(pipe_fd[1], line, read);
		}
		free_args(environ);
		free(environ);
		close(pipe_fd[1]); /* Cerramos el extremo de escritura de la tubería */
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
		lseek(STDIN_FILENO, 0, SEEK_END);
		free(line);
	}
}
