#include "shell.h"

/**
 * get_env_var - Retrieves the value of an environment variable.
 * @name: The name of the environment variable.
 *
 * Return: The value of the environment variable, or NULL if not found.
 *
 * This function searches for an environment variable with the given name
 * and returns its value.
 */
char *get_env_var(const char *name)
{
	char **env = environ;
		while (*env)
	{
		if (_strncmp(*env, name, _strlen(name)) == 0 && (*env)[_strlen(name)] == '=')
		{
			return (*env + _strlen(name) + 1); /* Devuelve el valor después del '=' */
		}
		env++;
	}
	return (NULL); /* Variable de entorno no encontrada */
}

/**
 * count_env_var - set variable in environement
 * @env: pointer to extern environement variable
 * @args: An array of arguments passed to the 'unsetenv' command.
 * @flag_var_env_found: flag to 1 if environement variable already exist
 *
 * Return: number of variables
 */
int count_env_var(char **env, char *args[], int *flag_var_env_found)
{
	int num_vars = 0;
	char *tmp = NULL;
	char *tmp2 = NULL;

	while (*env)
	{
		tmp = _strdup(*env);
		tmp2 = strtok(tmp, "=");
		if (_sstrcmp(tmp2, args[1]) == 0)
			*flag_var_env_found = 1;
		free(tmp);
		num_vars++;
		env++;
	}
	env = environ;
	return (num_vars);
}

/**
 * set_env_var - set variable in environement
 * @env: pointer to extern environement variable
 * @args: An array of arguments passed to the 'unsetenv' command.
 * @size_malloc: size of new environement
 * @new_env_var: name of new variable
 * @flag_var_env_found: flag to 1 if environement variable already exist
 *
 * Return: status 0 on success, 1 on failure.
 */
int set_env_var(char **env, char *args[], int size_malloc,
char *new_env_var, int flag_var_env_found)
{
	char **new_environ = NULL;
	int i = 0, status = 0;
	char *tmp = NULL;
	char *tmp2 = NULL;

	new_environ = (char **)malloc((size_malloc) * sizeof(char *));
	init_array_of_strings(new_environ, (size_malloc));
	if (new_environ == NULL)
	{
		perror("malloc"); /* Mostrar error si la asignación de memoria falla */
		status = 1;
		return (status);
	}
	while (*env)
	{
		tmp = _strdup(*env);
		tmp2 = strtok(tmp, "=");
		if (_sstrcmp(tmp2, args[1]) == 0)
			new_environ[i] = new_env_var;
		else
			new_environ[i] = _strdup(*env);
		free(tmp);
		i++;
		env++;
	}
	if (!flag_var_env_found)
	{
		new_environ[i] = new_env_var;/* add new var de entorno al arreglo */
		i++;
	}
	return (status);
}
/**
 * shell_unsetenv - Remove an environment variable.
 * @args: An array of arguments passed to the 'unsetenv' command.
 *
 * Return: 0 on success, -1 on failure.
 *
 * This function removes the specified environment variable from the shell's
 * environment.
 */
int shell_unsetenv(char *args[])
{
	char **new_environ = NULL;
	char **env = environ;
	char *tmp = NULL;
	char *tmp2 = NULL;
	int i = 0, num_vars = 0, num_args = 0, flag_var_env_found = 0;

	while (args[num_args] != NULL)
		num_args++;
	if (num_args == 1)
		return (0);

	num_vars = count_env_var(env, args, &flag_var_env_found);
	if (flag_var_env_found)
	{
		new_environ = (char **)malloc((num_vars) * sizeof(char *));
		while (*env)
		{
			tmp = _strdup(*env);
			tmp2 = strtok(tmp, "=");

			if (_sstrcmp(tmp2, args[1]) != 0)
			{
				new_environ[i] = _strdup(*env);
				i++;
			}
			free(tmp);
			env++;
		}
		new_environ[i] = NULL;
		env = environ;
		free_args(env);
		free(env);
		environ = new_environ;
	} else
	{
		print_error("variable does not exist in env");
		return (-1);
	}
	return (0);
}

/**
 * shell_setenv - Set or update an environment variable.
 * @args: An array of arguments passed to the 'setenv' command.
 *
 * Return: 0 on success, 1 on failure.
 *
 * This function sets or updates an environment variable with the given name
 * and value.
 * It supports the 'setenv VARIABLE_NAME VALUE' command format.
 */
int shell_setenv(char *args[])
{
	int flag_var_env_found = 0, size_malloc = 0, num_vars = 0, status = 0;
	char **env = environ; /* Obtener arreglo de variables entorno existentes */
	char **new_environ = NULL;
	char *new_env_var = NULL;

	if (args[1] != NULL && args[2] != NULL) /* si se dan suficientes argumentos */
	{
		size_malloc = _strlen(args[1]) + _strlen(args[2]) + 2;
		new_env_var = (char *)malloc(size_malloc); /* 2: "=" + "\0" */
		init_string(new_env_var, size_malloc);
		size_malloc = 0;
		if (new_env_var == NULL)
		{
			perror("malloc");/* Mostrar error si la asignación de memoria falla */
			return (1);
		}
		sprintf(new_env_var, "%s=%s", args[1], args[2]); /* hacer cadena env */
		num_vars = count_env_var(env, args, &flag_var_env_found);
		env = environ;/* Reiniciar el puntero al arreglo de variables de entorno */
		if (flag_var_env_found)
			size_malloc = num_vars + 1;
		else
			size_malloc = num_vars + 2;
		set_env_var(env, args, size_malloc, new_env_var, flag_var_env_found);

		env = environ;/* Reiniciar el puntero al arreglo de variables de entorno */
		free_args(env);
		free(env);
		environ = new_environ;/* Act variable global 'env' reflejar nuevo arreglo */
	}
	else
	{
		fprintf(stderr, "Usage: setenv VARIABLE_NAME VALUE\n");
		status = 1;
		return (status);
	}
	return (status);
}
