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

	char current_directory[MAX_INPUT_LENGTH];
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
			fprintf(stderr, "cd: No se ha definido la variable OLDPWD\n");
			return (-1);
		}
		printf("%s\n", oldpwd);

		if (chdir(oldpwd) != 0)
		{
			perror("cd");
			return (-1);
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
	char oldpwd_variable[MAX_INPUT_LENGTH + 7];  /* // +7 for "OLDPWD=" */

	snprintf(oldpwd_variable, sizeof(oldpwd_variable), "OLDPWD=%s", current_directory);

	char **env = environ;
	while (*env)
	{
		if (_strncmp(*env, "OLDPWD=", 7) == 0)
		{
			/* Replace the existing OLDPWD entry */
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
	if (args[1] != NULL && args[2] != NULL) /* Verificar si se proporcionan suficientes argumentos */
	{
		/* Crear una nueva cadena que contendrá la nueva variable de entorno en el formato "NOMBRE=VALOR" */
		char *new_env_var = malloc(_strlen(args[1]) + _strlen(args[2]) + 2);
		if (new_env_var == NULL)
		{
			perror("malloc");/* Mostrar error si la asignación de memoria falla */
			return (1);
		}
		sprintf(new_env_var, "%s=%s", args[1], args[2]); /* Construir la cadena de variable de entorno */

		char **env = environ; /* Obtener el arreglo de variables de entorno existentes */
		int num_vars = 0;
		while (*env)
		{
			num_vars++; /* Contar el número de variables de entorno existentes */
			env++;
		}
	/*Crear un nuevo arreglo de variables de entorno con espacio para la nueva variable y NULL adicional*/
		char **new_environ = malloc((num_vars + 2) * sizeof(char *));
		if (new_environ == NULL)
		{
			perror("malloc"); /* Mostrar error si la asignación de memoria falla */
			free(new_env_var);
			return (1);
		}

		env = environ;/* Reiniciar el puntero al arreglo de variables de entorno */
		int i = 0;
		while (*env)
		{
			new_environ[i] = _strdup(*env);
			i++;
			env++;
		}

		new_environ[i++] = new_env_var;/* Agregar la nueva variable de entorno al arreglo */
		new_environ[i] = NULL; /* Marcar el final del arreglo con NULL */

		environ = new_environ;/* Actualizar la variable global 'environ' para reflejar el nuevo arreglo */

		printf("Variable set: %s\n", new_env_var);

		free(new_env_var); /* Liberar new_env_var después de agregarlo a new_environ */
		for (int j = 0; j < num_vars; j++)
		{
			free(new_environ[j]); /* Liberar las variables existentes copiadas */
		}
		free(new_environ); /* Liberar new_environ */

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
