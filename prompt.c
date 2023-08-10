#include "shell.h"

char *read_input()
{
char *line = NULL;
size_t char_read = 0;
size_t bufsize = MAX_INPUT_LENGTH;
char_read = getline(&line, &bufsize, stdin);
printf("DEBUG: line: %p\n", line);
// if (char_read == 0)
// 	line[0] = '\0';
return (line);
}

void run_shell_loop(void)
{
	char *input = NULL;
	char *args[MAX_ARGS] = {NULL};
	int num_args = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO)) {
			printf("$ "); /* Solo mostramos el prompt en modo interactivo */
		}
		input = read_input();
		if (input == NULL)
		{
			printf("\n");
			break; /* Ctrl+D or EOF */
		}
		num_args = tokenize_input(input, args);
		if (num_args == 0)
		{
			if (isatty(STDIN_FILENO))
			{
				free(input);
				free_args(args);
				continue; /*Empty line*/
			} else
				break;
		}
		if (_sstrcmp(args[0], "exit") == 0)
		{
			break;
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
		free_args(args);
	}
	free(input);
	free_args(args);
}
