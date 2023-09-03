#include "shell.h"
/**
 * read_input - Reads a line of input from the user.
 *
 * Return: A dynamically allocated string containing the user's input,
 * or NULL in case of an error or end of file.
 */
/* Leer una línea de entrada desde el usuario */
char *read_input()
{
	char *line = NULL;
	ssize_t read_size;
	size_t bufsize = MAX_INPUT_LENGTH;

	read_size = getline(&line, &bufsize, stdin);
	if (read_size == -1)
	{
		free(line);
		return (NULL); /* Error o final del archivo */
	}

	return (line);
}

/**
 * send_command - send command to executor.
 * @command: string of the command to send
 * @line: complete line with possibly multiple command in it
 * @status: status of last executed command
 * @line_number: number of the line of the command
 * @flag_logic_func: boolean flag to avoid exit
 * in case of logic operators
 * Return: status of executed command
 */
int send_command(char *command, char *line, int status,
				 int line_number, int flag_logic_func)
{
	int num_args = 0;
	char *args[MAX_ARGS] = {NULL};

	num_args = tokenize_input(command, args);
	if (num_args == 0)
	{
		free_args(args);
		return (0);
	}
	if (_sstrcmp(args[0], "exit") == 0)
	{
		free(line);
		shell_exit(args, line_number, status);
	}
	else if (_sstrcmp(args[0], "cd") == 0)
		shell_cd(args); /* Ejecutar el comando "cd" */
	else if (_sstrcmp(args[0], "env") == 0)
		shell_env(args); /* Ejecutar el comando "env" */
	else if (_sstrcmp(args[0], "setenv") == 0)
		shell_setenv(args); /* Ejecutar el comando "setenv" */
	else if (_sstrcmp(args[0], "unsetenv") == 0)
		shell_unsetenv(args); /* Ejecutar el comando "unsetenv" */
	else
	{
		if (status == 127 && !flag_logic_func)
		{
			free(line);
			free_args(args);
			free_args(environ);
			free(environ);
			exit(0);
		}
		status = execute_command(args, line_number, command);
	}
	free_args(args);

	return (status);
}

/**
 * line_to_command - tokenisation of the line in multiple commands
 * @commands: string of the command to send
 * @line: complete line with possibly multiple command in it
 * @ptr_logic: pointer to last logic operator character
 */
void line_to_command(char *commands[], char *line, char *ptr_logic)
{
	char *token = NULL;
	int i = 0;
	char delim[2];

	delim[0] = ptr_logic[0];
	delim[1] = '\0';

	token = strtok(line, delim);
	for (i = 0; token != NULL; i++)
	{
		commands[i] = token;
		token = strtok(NULL, delim);
	}
}

/**
 * tokenize_line - divide line in multiple commands if necessary.
 * @commands: string of the command to send
 * @line: complete line with possibly multiple command in it
 * @status: status of last executed command
 * in case of logic operators
 * Return: a number that match a logic operator or not
 */
int tokenize_line(char *commands[], char *line, int *status)
{
	int flag = 0;
	char *ptr_and = _strchr(line, '&');
	char *ptr_or = _strchr(line, '|');
	char *ptr_coma = _strchr(line, ';');

	if (ptr_coma != NULL)
	{
		line_to_command(commands, line, ptr_coma);
	}
	else if (ptr_and)
	{
		if (ptr_and[1] == '&')
		{
			flag = 1;
			line_to_command(commands, line, ptr_and);
		}
	}
	else if (ptr_or)
	{
		if (ptr_or[1] == '|')
		{
			flag = 2;
			line_to_command(commands, line, ptr_or);
			*status = 1;
		}
	}
	else
	{
		commands[0] = line;
		commands[1] = NULL;
	}
	return (flag);
}

/**
 * handle_logic - execute command with conditions
 * of logic operators
 * @commands: string of the command to send
 * @logic_flag: number that correspond to a logic operator or not
 * @line: complete line with possibly multiple command in it
 * @line_number: number of the line of the command
 * @status: status of last executed command
 * in case of logic operators
 */
void handle_logic(char *commands[], int logic_flag, char *line,
				  int line_number, int status)
{
	int j = 0;

	for (j = 0; commands[j]; j++)
	{
		if (logic_flag == 1)
		{
			if (status == 0)
				status = send_command(commands[j], line, status, line_number, logic_flag);
			else
			{
				logic_flag = 0;
				while (commands[j] != NULL)
					j++;
			}
		}
		else if (logic_flag == 2)
		{
			if (status != 0)
				status = send_command(commands[j], line, status, line_number, logic_flag);
			else
			{
				logic_flag = 0;
				while (commands[j] != NULL)
					j++;
			}
		}
		else
			status = send_command(commands[j], line, status, line_number, 0);
	}
}
