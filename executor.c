#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void execute_command(char *args[], int line_number)
{
	/*char *path = _getenv("PATH");
	char *path_copy = _strdup(path);
	char *dir = strtok(path_copy, ":");*/
	Env_t *PATH = NULL;
	Env_l_t *env_lists = load_env(environ);
	pid_t pid;

	while(strcmp(env_lists->name, "PATH"))
	{
		env_lists = env_lists->next;
	}
	PATH = env_lists->list;

	pid = fork();
	if (pid == 0)
	{
		char *env[] = {NULL};
		if (access(args[0], X_OK) == 0)
		{
			execve(args[0], args, env);
			perror("Error executing command");
			exit(EXIT_FAILURE);
		}

		while (PATH != NULL)
		{
			char executable_path[MAX_INPUT_LENGTH];
				snprintf(executable_path, sizeof(executable_path), "%s/%s", PATH->value, args[0]);
			if (access(executable_path, X_OK) == 0)
			{
				execve(executable_path, args, env);
				perror("Error executing command");
				exit(EXIT_FAILURE);
			}
			/*dir = strtok(NULL, ":");*/
			PATH = PATH->next;
		}
		/*free(path_copy);*/
		fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, args[0]);
		exit(127);
	}
	else if (pid < 0)
	{
		print_error("Error forking");
	}
	else
	{
		/*free(path_copy);*/
		wait(NULL);
	}

	free_list_list(env_lists);
}
