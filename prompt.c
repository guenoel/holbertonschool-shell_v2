	#include "shell.h"

	char *read_input()
	{
	char *line = NULL;
	size_t bufsize = 0;
		getline(&line, &bufsize, stdin);
	return (line);
	}

void run_shell_loop(void)
{
	char *input;
	char *args[MAX_ARGS];
	int num_args;
		while (1)
	{
		printf("$ ");
		input = read_input();
		if (input == NULL)
		{
			printf("\n");
			break; /*Ctrl+D or EOF*/
		}
		num_args = tokenize_input(input, args);
		if (num_args == 0)
		{
			free(input);
			continue; /*Empty line*/
		}
		if (_sstrcmp(args[0], "exit") == 0)
		{
			shell_exit(args);
		}
		else if (_sstrcmp(args[0], "cd") == 0)
		{
			shell_cd(args);
		}
		else if (_sstrcmp(args[0], "env") == 0)
		{
			shell_env(args);
		}
		else if (_sstrcmp(args[0], "setenv") == 0)
		{
			shell_setenv(args);
		}
		else if (_sstrcmp(args[0], "unsetenv") == 0)
		{
			shell_unsetenv(args);
		}
		else
		{
			execute_command(args);
		}
		free(input);
	}
}
