#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void execute_command(char *args[])
{
	pid_t pid = fork();
	if (pid == 0)
	{

		char *env[] =
		{
			"PATH=/usr/bin:/bin",
			"HOME=/home/user",
			NULL
		};

		printf("args:");
		for (int i = 0; args[i] != NULL; i++)
		{
			printf(" %s", args[i]);
		}
		printf("\n");

		printf("env:");
		for (int i = 0; env[i] != NULL; i++)
		{
			printf(" %s", env[i]);
		}
		printf("\n");

		execve(args[0], args, env);
		printf("Executing command: %s\n", args[0]);

		print_error("Error executing command");
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

/*
void execute_command(char *args[]) {
	pid_t pid = fork();
	if (pid == 0) { // Child process
		execvp(args[0], args);
		print_error("Error executing command");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		print_error("Error forking");
	} else {
		wait(NULL);
	}
}*/
