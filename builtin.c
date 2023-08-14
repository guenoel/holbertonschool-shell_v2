#include "shell.h"
#include <stdlib.h>

#define UNUSED(x) (void)(x)

int shell_cd(char *args[])
{
	if (args[1] == NULL)
	{
		Env_l_t *env_lists = load_env(environ);
		while(strcmp(env_lists->name, "HOME"))
		{
			printf("%s, ", env_lists->name);
			env_lists = env_lists->next;
		}
		printf("%s\n", env_lists->name);
		printf("%s\n", env_lists->list->value);
		chdir("/root");
		free_list_list(env_lists);
		
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
		}
		chdir(args[1]);
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
	char **env = environ;
	UNUSED(args);
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}

	// /*test for load_env function*/
	// Env_l_t *env_lists = load_env(environ);

	// Env_l_t *current_list = env_lists;
    // while (current_list != NULL) 
	// {
    //     printf("Variable: %s\n", current_list->name);

    //     Env_t *current = current_list->list;
    //     while (current != NULL) {
    //         printf("Value: %s\n", current->value);
    //         current = current->next;
    //     }

    //     current_list = current_list->next;
    // }
	// free_list_list(env_lists);
	// /*end of test*/

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
	int i;
	for (i = 0; args[i] != NULL; i++)
	{
		free(args[i]);
	}
}
