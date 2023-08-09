#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void execute_command(char *args[])
{
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
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *dir = strtok(path_copy, ":");
	while (dir != NULL) {
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
	print_error("Command not found");
	exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		print_error("Error forking");
	}
	else
	{
		wait(NULL);
	}
}
