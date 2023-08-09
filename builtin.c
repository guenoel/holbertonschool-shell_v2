#include "shell.h"
#include <stdlib.h>

#define UNUSED(x) (void)(x)

int shell_cd(char *args[])
{
	if (args[1] == NULL)
	{
		chdir(_getenv("HOME"));
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
		}
	}
	return (1);
}

int shell_exit(char *args[])
{
	UNUSED(args);
	exit(0);
}

int shell_env(char *args[])
{
	UNUSED(args);
	char **env = environ;
		while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (1);
}

int shell_unsetenv(char *args[])
{
	if (args[1] != NULL)
	{
		if (unsetenv(args[1]) != 0)
		{
			perror("unsetenv");
		}
	}
	return (1);
}

int shell_setenv(char *args[])
{
	if (args[1] != NULL && args[2] != NULL)
	{
		if (setenv(args[1], args[2], 1) != 0)
		{
			perror("setenv");
		}
	}
	return (1);
}

void free_args(char *args[])
{
	for (int i = 0; args[i] != NULL; i++)
	{
		free(args[i]);
	}
}
