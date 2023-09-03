#include "shell.h"

/**
* format_command - format command before execution
* @args: An array of strings representing the command and its arguments
* @input: Command input
* @line_number: Line number in the shell script where the command is executed
* @path_copy: copy of a path from environement
*/
void format_command(char *args[], char *input,
					int line_number, char *path_copy)
{
	int i = 0;
	char *input_file = NULL;
	char *output_file = NULL;
	char *heredoc_delimiter = NULL;

	for (i = 0; args[i] != NULL; i++)
	{
		if (_sstrcmp(args[i], "<") == 0)
		{
			input_file = args[i + 1];
			free(args[i]);
			args[i] = NULL;
			handle_input_redirection(input_file, line_number, args, path_copy, input);
			free(input_file);
		}
		else if (_sstrcmp(args[i], ">") == 0)
		{
			output_file = args[i + 1];
			free(args[i]);
			args[i] = NULL;
			handle_output_redirection(output_file);
			free(output_file);
		}
		else if (_sstrcmp(args[i], ">>") == 0)
		{
			output_file = args[i + 1];
			free(args[i]);
			args[i] = NULL;
			handle_double_output_redirection(output_file);
			free(output_file);
		}
		else if (_sstrcmp(args[i], "<<") == 0)
		{
			heredoc_delimiter = args[i + 1];
			free(args[i]);
			args[i] = NULL;
			handle_heredoc(heredoc_delimiter);
			free(heredoc_delimiter);
		}
	}
}

/**
* format_command - format command before execution
* @line_number: Line number in the shell script where the command is executed
* @args: An array of strings representing the command and its arguments
* @path_copy: copy of a path from environement
* @input: Command input
* @env: pointer to extern variable environ
* @executable_path: right path to execute command
* @saved_stdin: number of fd stdin
* @saved_stdout: number of stdout
* Return: exit status number
*/
int fork_to_execute(int line_number, char *args[],
char *path_copy, char *input, char **env,
char executable_path[], int saved_stdin, int saved_stdout)
{
	pid_t pid = 0;
	int status;

	pid = fork();
	if (pid == 0)
	{
		format_command(args, input, line_number, path_copy);
		free(path_copy);
		execve(executable_path, args, env);
		perror("Error executing command");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		print_error("Error forking");
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
			printf("Child process did not exit normally\n");
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}

	return (0);
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
	char *dir = NULL;
	char *path = NULL;
	char *path_copy = NULL;
	int saved_stdin, saved_stdout, status;
	char **env = environ;
	char executable_path[MAX_INPUT_LENGTH];

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	path = _getenv("PATH");
	path_copy = _strdup(path);
	dir = strtok(path_copy, ":");

	if (access(args[0], X_OK) == -1 || !_strchr((args[0]), '/'))
	{
		while (dir != NULL)
		{
			sprintf(executable_path, "%s/%s", dir, args[0]);
			if (access(executable_path, X_OK) == 0)
				break;
			dir = strtok(NULL, ":");
		}
		if (dir == NULL)
		{
			fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, args[0]);
			free(path_copy);
			return (127);
		}
	}
	else
		_strcpy(executable_path, args[0]);

	status = fork_to_execute(line_number, args, path_copy, input,
env, executable_path, saved_stdin, saved_stdout);
	return (status);
}
