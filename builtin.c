#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define UNUSED(x) (void)(x)
#define MAX_INPUT_LENGTH 1024
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
/* Cambiar el directorio actual */
int shell_cd(char *args[])
{
	static char previous_directory[MAX_INPUT_LENGTH] = "";
	char *oldpwd_variable= (char *)malloc((MAX_INPUT_LENGTH + 7) * sizeof(char *));  /* // +7 for "OLDPWD=" */
	char **env = environ;
	char current_directory[MAX_INPUT_LENGTH] = "";
	if (getcwd(current_directory, sizeof(current_directory)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
	if (args[1] == NULL || _sstrcmp(args[1], "~") == 0)
	{
		char *home_directory = get_env_var("HOME");
		if (home_directory == NULL)
		{
			return (-1);
		}
		if (chdir(home_directory) != 0)
		{
			perror("cd");
			return (-1);
		}
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
{
	char *oldpwd = get_env_var("OLDPWD"); /* Obtener el valor actual de OLDPWD */
	if (oldpwd == NULL)
	{
		char *pwd = get_env_var("PWD"); /* Obtener el valor actual de PWD */
		if (pwd == NULL)
		{
			fprintf(stderr, "cd: No se ha definido la variable OLDPWD ni PWD\n");
			return (-1);
		}
		printf("%s\n", pwd);
		if (chdir(pwd) != 0)
		{
			perror("cd");
			return (-1);
		}
	}
	else
	{
		printf("%s\n", oldpwd);
		if (chdir(oldpwd) != 0)
		{
			perror("cd");
			return (-1);
		}
	}
}
	else
	{
		if (chdir(args[1]) != 0)
		{
			fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", args[1]);
			return (-1);
		}
	}
	/* Actualizar OLDPWD al valor del directorio actual */
	sprintf(oldpwd_variable, "OLDPWD=%s", current_directory);
	while (*env)
	{
		printf("condition: %d\n", _strncmp(*env, "OLDPWD=", 7));
		if (_strncmp(*env, "OLDPWD=", 7) == 0)
		{
			/* Replace the existing OLDPWD entry */
			printf("cuu");
			free(*env);
			*env = oldpwd_variable;
			break;
		}
		env++;
	}
	_strcpy(previous_directory, current_directory);
	return (0);
}
/* Salir de la shell */
int shell_exit(char *args[])
{
	UNUSED(args);
	exit(0); /* Salir del programa con éxito */
}
/* Mostrar las variables de entorno */
int shell_env(char *args[])
{
	char **env = environ; /* Obtener el arreglo de variables de entorno */
	UNUSED(args);
	while (*env)
	{
		printf("%s\n", *env); /* Imprimir cada variable de entorno */
		env++;
	}
	return (1); /* Indicar que el comando se ejecutó correctamente */
}
/* Eliminar una variable de entorno */
int shell_unsetenv(char *args[])
{
	if (args[1] != NULL)
	{
		if (unsetenv(args[1]) != 0) /* Eliminar la variable de entorno especificada */
		{
			perror("unsetenv"); /* Mostrar mensaje de error si unsetenv falla */
		}
	}
	return (1); /* Indicar que el comando se ejecutó correctamente */
}
int shell_setenv(char *args[])
{
	char **new_environ = NULL;
	int flag_var_env_found = 0;
	char **env = environ; /* Obtener el arreglo de variables de entorno existentes */
	char *new_env_var = NULL;
	int num_vars = 0;
	int size_malloc = 0;
	char *tmp = NULL;
	char *tmp2 = NULL;
	int i = 0;

	if (args[1] != NULL && args[2] != NULL) /* Verificar si se proporcionan suficientes argumentos */
	{
/* STEP 1 - PREPARACION DEL DATA, DE num_vars Y DEL FLAG*/
		/* Crear una nueva cadena que contendrá la nueva variable de entorno en el formato "NOMBRE=VALOR" */
		size_malloc = _strlen(args[1]) + _strlen(args[2]) + 2;
		new_env_var = (char *)malloc(size_malloc); /* 2: "=" + "\0" */
		init_string(new_env_var, size_malloc);
		size_malloc = 0;
		if (new_env_var == NULL)
		{
			perror("malloc");/* Mostrar error si la asignación de memoria falla */
			return (1);
		}
		sprintf(new_env_var, "%s=%s", args[1], args[2]); /* Construir la cadena de variable de entorno */

		while (*env)
		{
			tmp = _strdup(*env);
			tmp2 = strtok(tmp, "=");
			if (_sstrcmp(tmp2, args[1]) == 0)
				flag_var_env_found = 1;
			free(tmp);
			num_vars++; /* Contar el número de variables de entorno existentes */
			env++;
		}
		env = environ;/* Reiniciar el puntero al arreglo de variables de entorno */
/* STEP 2 CREACION DEL NEW ENV*/
		/*Crear un nuevo arreglo de variables de entorno con espacio para la nueva variable (si no existe) y NULL adicional*/
		if (flag_var_env_found)
			size_malloc = num_vars + 1;
		else
			size_malloc = num_vars + 2;
		new_environ = (char **)malloc((size_malloc) * sizeof(char *));
		init_array_of_strings(new_environ, (size_malloc));
		if (new_environ == NULL)
		{
			perror("malloc"); /* Mostrar error si la asignación de memoria falla */
			return (1);
		}
/* STEP 3 COPY FROM OLD TO NEW ENV*/

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
			new_environ[i] = new_env_var;/* Agregar la nueva variable de entorno al arreglo */
			i++;
		}

/* STEP 4 FREE OLD ENV*/
		env = environ;/* Reiniciar el puntero al arreglo de variables de entorno */
		free_args(env);
		free(env);
/* ASIGNACION DE NUEVO ENVIRON */
		environ = new_environ;/* Actualizar la variable global 'environ' para reflejar el nuevo arreglo */
	}
	else
	{
		fprintf(stderr, "Usage: setenv VARIABLE_NAME VALUE\n");
		return (1);
	}
	return (0);
}

/* Liberar la memoria ocupada por el arreglo de argumentos */
void free_args(char *args[])
{
	int i;
	for (i = 0; args[i] != NULL; i++)
	{
		free(args[i]); /* Liberar la memoria de cada argumento */
	}
}

/* Function to copy the environ array into a new variable */
void malloc_environ() {
	char **new_env;
	int num_vars = 0;
	int i = 0;

	/* Count the number of variables in environ */
	while (environ[num_vars] != NULL) {
		num_vars++;
	}

	/* Allocate memory for the new array of pointers */
	new_env = (char **)malloc((num_vars + 1) * sizeof(char *));
	if (new_env == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	/* Copy each string from environ to the new array using strdup */
	for (i = 0; i < num_vars; i++) {
		new_env[i] = _strdup(environ[i]);
		if (new_env[i] == NULL) {
			perror("strdup");
			exit(EXIT_FAILURE);
		}
	}

	/* Terminate the new array with a NULL pointer */
	new_env[num_vars] = NULL;

	environ = new_env;
}

int init_array_of_strings(char **array_of_strings, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		array_of_strings[i] = NULL;
	}
	return (0);
}

int init_string(char *string, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		string[i] = '\0';
	}
	return (0);
}
