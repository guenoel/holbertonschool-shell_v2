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
	char current_directory[MAX_INPUT_LENGTH];
	char *old_pwd = get_env_var("PWD");
	char **env = environ;

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
			fprintf(stderr, "cd: No HOME variable set\n");
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
		if (old_pwd == NULL)
		{
			fprintf(stderr, "cd: No OLDPWD variable set\n");
			return (-1);
		}
		if (chdir(old_pwd) != 0)
		{
			perror("cd");
			return (-1);
		}
		printf("%s\n", old_pwd);
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", args[1]);
			return (-1);
		}
	}

	if (old_pwd)
	{
	/* Buscar la variable OLDPWD en el arreglo de variables de entorno */
	/* char **env = environ; */
	while (*env)
	{
		if (_strncmp(*env, "OLDPWD=", 7) == 0)
		{
			/* Reemplazar el valor de OLDPWD con el valor almacenado en old_pwd */
			*env = malloc(_strlen(old_pwd) + 9);  /* 7 (OLDPWD=) + 2 (NULL terminador y '=') */
			if (*env)
			{
				sprintf(*env, "OLDPWD=%s", old_pwd);
			}
			break; /* Salir del ciclo al encontrar OLDPWD */
		}
		env++;
	}
}
	/* Buscar la variable PWD en el arreglo de variables de entorno */
	/* char **env = environ; */
	while (*env)
	{
		if (_strncmp(*env, "PWD=", 4) == 0)
		{
			/* Reemplazar el valor de PWD con el valor almacenado en current_directory */
			*env = malloc(_strlen(current_directory) + 6); /* 4 (PWD=) + 2 (NULL terminador y '=') */
			if (*env)
			{
				sprintf(*env, "PWD=%s", current_directory);
			}
			break; /* Salir del ciclo al encontrar PWD */
		}
		env++;
	}
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

/* Establecer una nueva variable de entorno o modificar una existente */
int shell_setenv(char *args[])
{
	char **env = environ;
	int num_vars = env - environ;

	if (args[1] != NULL && args[2] != NULL)
	{
		char *new_env_var = malloc(strlen(args[1]) + strlen(args[2]) + 2);
		if (new_env_var == NULL)
		{
			perror("malloc");
			return (1); /* Retorna 1 para indicar éxito en este caso */
		}
		sprintf(new_env_var, "%s=%s", args[1], args[2]);

		/* char **env = environ; */
		while (*env)
		{
			if (strncmp(*env, args[1], strlen(args[1])) == 0)
			{
				/* Reemplazar la variable de entorno existente */
				free(*env);
				*env = new_env_var;
				return (1); /* Retorna 1 para indicar éxito */
			}
			env++;
		}
		/* Si la variable de entorno no existe, agregamos una nueva */
		/* int num_vars = env - environ; */
		environ = realloc(environ, (num_vars + 2) * sizeof(char *));
		if (environ == NULL)
		{
			perror("realloc");
			free(new_env_var);
			return (1); /* Retorna 1 para indicar éxito en este caso */
		}
		environ[num_vars] = new_env_var;
		environ[num_vars + 1] = NULL;
		return (1); /* Retorna 1 para indicar éxito */
	}
	return (1); /* Retorna 1 para indicar éxito por defecto */
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
