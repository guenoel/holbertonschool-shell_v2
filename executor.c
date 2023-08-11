#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void execute_command(char *args[], int line_number)
{
	char *path = _getenv("PATH");
	char *path_copy = _strdup(path);
	char *dir = strtok(path_copy, ":");

	pid_t pid = fork();
	if (pid == 0)
	{
		char *env[] = {NULL};
			if (access(args[0], X_OK) == 0)
		{
			execve(args[0], args, env);
			perror("Error executing command");
			exit(EXIT_FAILURE);
		}

			while (dir != NULL)
		{
			char executable_path[MAX_INPUT_LENGTH];
				snprintf(executable_path, sizeof(executable_path), "%s/%s", dir, args[0]);
			if (access(executable_path, X_OK) == 0)
			{
				execve(executable_path, args, env);
				perror("Error executing command");
				exit(EXIT_FAILURE);
			}
			dir = strtok(NULL, ":");
		}
		free(path_copy);
		fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, args[0]);
		exit(127);
	}
	else if (pid < 0)
	{
		print_error("Error forking");
	}
	else
	{
		free(path_copy);
		wait(NULL);
	}
}
