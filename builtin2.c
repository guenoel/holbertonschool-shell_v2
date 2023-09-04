#include "shell.h"

/**
 * cd_old - print and change old directory
 *
 * Return: status number 0 on success, -1 on failure.
 */
int cd_old(void)
{
	int status = 0;
	char *oldpwd = get_env_var("OLDPWD"); /* Obtener el valor actual de OLDPWD */
	printf("oldpwd: %s\n", oldpwd);
	if (oldpwd == NULL)
	{
		char *pwd = get_env_var("PWD"); /* Obtener el valor actual de PWD */
		printf("pwd: %s\n", pwd);
		if (pwd == NULL)
		{
			fprintf(stderr, "cd: No se ha definido la variable OLDPWD ni PWD\n");
			status = -1;
			return (status);
		}
		printf("%s\n", pwd);
		if (chdir(pwd) != 0)
		{
			perror("cd");
			status = -1;
			return (status);
		}
	} else
	{
		printf("%s\n", oldpwd);
		if (chdir(oldpwd) != 0)
		{
			perror("cd");
			status = -1;
			return (status);
		}
	}
	return (status);
}

/**
 * cd_home - print and change old directory
 *
 * Return: status number 0 on success, -1 on failure.
 */
int cd_home(void)
{
	int status = 0;

	char *home_directory = get_env_var("HOME");

	if (home_directory == NULL)
	{
		status = -1;
		return (status);
	}
	if (chdir(home_directory) != 0)
	{
		perror("cd");
		status = -1;
		return (status);
	}

	return (status);
}

/**
 * shell_cd - Change the current working directory.
 * @args: An array of arguments passed to the 'cd' command.
 *
 * Return: 0 on success, -1 on failure.
 *
 * This function changes the current working directory based on the provided
 * arguments.
 * It supports the 'cd', 'cd ~', 'cd -', and 'cd [directory]' commands.
 */
int shell_cd(char *args[])
{
	static char previous_directory[MAX_INPUT_LENGTH] = "";
	char current_directory[MAX_INPUT_LENGTH] = "";
	int i = 0, status = 0;
	char *argsetenv[3] = {NULL};

	for (i = 0; i < (MAX_INPUT_LENGTH); i++)
	{
		current_directory[i] = '\0';
		previous_directory[i] = '\0';
	}

	if (getcwd(current_directory, sizeof(current_directory)) == NULL)
	{
		perror("getcwd");
		status = -1;
		return (status);
	}
	if (args[1] == NULL || _sstrcmp(args[1], "~") == 0)
	{
		status = cd_home();
		if (status != 0)
			return status;
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		status = cd_old();
		if (status != 0)
			return status;
	} else
	{
		if (chdir(args[1]) != 0)
		{
			fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", args[1]);
			status = -1;
			return (status);
		}
	}
	argsetenv[0] = "setenv";
	argsetenv[1] = "OLDPWD";
	argsetenv[2] = current_directory;
	shell_setenv(argsetenv);

	_strcpy(previous_directory, current_directory);
	return (status);
}

/**
 * shell_exit - Exit the shell.
 * @args: An array of arguments passed to the 'exit' command.
 * @line_number: The current line number.
 * @child_status: The status of the last child process.
 *
 * Return: The status of the last child process
 * This function exits the shell with the specified exit status.
 * It handles the 'exit' command and accepts an optional exit status code.
 */
int shell_exit(char *args[], int line_number, int child_status)
{
	char *endptr = NULL;
	long num = 0;

	free_args(environ);
	free(environ);
	if (args[1] == NULL)
	{
		free_args(args);
		exit(child_status);
	}
	num = _strtol(args[1], &endptr, 10);
	if (*endptr == '\0')
	{
		if (num < 0)
		{
			fprintf(stderr, "./hsh: %d: %s: Illegal number: %s\n", line_number,
					args[0], args[1]);
			free_args(args);
			exit(2);
		}
		else if (num == 1000)
		{
			free_args(args);
			exit(232);
		}
		else
		{
			free_args(args);
			exit(num);
		}
	} else
	{
		fprintf(stderr, "./hsh: %d: %s: Illegal number: %s\n", line_number,
				args[0], args[1]);
		free_args(args);
		exit(2);
	}
}

/**
 * shell_env - Display the environment variables.
 *
 * Return: 1 (success).
 *
 * This function displays all environment variables to the standard output.
 */
int shell_env(void)
{
	char **env = environ; /* Obtener el arreglo de variables de entorno */

	while (*env)
	{
		printf("%s\n", *env); /* Imprimir cada variable de entorno */
		env++;
	}
	return (1); /* Indicar que el comando se ejecutó correctamente */
}
