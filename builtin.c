#include "shell.h"
#include <stdlib.h>

#define UNUSED(x) (void)(x)

#include "shell.h"
#include <string.h>

/*Estructura para almacenar variables de entorno*/
typedef struct
{
	char *name;
	char *value;
} EnvironmentVariable;

/* Arreglo para almacenar las variables de entorno */
static EnvironmentVariable env_vars[MAX_ARGS];

/* Número de variables de entorno actuales*/
static int num_env_vars;

int shell_setenv(char *args[])
{
	int i;
	if (args[1] != NULL && args[2] != NULL)
	{
		if (num_env_vars < MAX_ARGS - 1)
		{
			/* Buscar si la variable ya existe en el arreglo*/
			for (i= 0; i < num_env_vars; i++)
			{
				if (_sstrcmp(env_vars[i].name, args[1]) == 0)
				{
					/* Actualizar el valor de la variable */
					free(env_vars[i].value);
					env_vars[i].value = _strdup(args[2]);
					return (1);
				}
			}

			/* Agregar la nueva variable al arreglo */
			env_vars[num_env_vars].name = _strdup(args[1]);
			env_vars[num_env_vars].value = _strdup(args[2]);
			num_env_vars++;

			return (1);
		}
		else
		{
			fprintf(stderr, "shell: too many environment variables\n");
			return (-1);
		}
	}

	return (1);
}

int shell_unsetenv(char *args[])
{
	int i;
	int j;

	if (args[1] != NULL)
	{
		for (i = 0; i < num_env_vars; i++)
		{
			if (_sstrcmp(env_vars[i].name, args[1]) == 0)
			{
				/* Liberar memoria y mover las variables siguientes hacia atrás */
				free(env_vars[i].name);
				free(env_vars[i].value);
				for (j = i; j < num_env_vars - 1; j++)
				{
					env_vars[j] = env_vars[j + 1];
				}
				num_env_vars--;
				return (1);
			}
		}
	}

	return (1);
}


int shell_cd(char *args[])
{
	char current_directory[MAX_INPUT_LENGTH]; /* Almacenar el directorio actual */
	char old_directory[MAX_INPUT_LENGTH]; /* Almacenar el directorio anterior */
	char *setenv_args_oldpwd[] = {"setenv", "OLDPWD", current_directory, NULL};
	char *setenv_args_pwd[] = {"setenv", "PWD", old_directory, NULL};

	if (getcwd(current_directory, sizeof(current_directory)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}

	if (args[1] == NULL)
	{
		char *home_directory = _getenv("HOME");
		if (home_directory == NULL)
		{
			fprintf(stderr, "cd: HOME variable not set\n");
			return (-1);
		}
		if (chdir(home_directory) != 0)
		{
			perror("chdir");
			return (-1);
		}
	}
	else if (_sstrcmp(args[1], "-") == 0)
	{
		if (_getenv("OLDPWD") == NULL)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (-1);
		}
		printf("%s\n", _getenv("OLDPWD"));
		if (chdir(_getenv("OLDPWD")) != 0)
		{
			perror("chdir");
			return (-1);
		}
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("chdir");
			return (-1);
		}
	}

	if (getcwd(old_directory, sizeof(old_directory)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}

	/* Actualizar las variables de entorno PWD y OLDPWD */
	if (shell_setenv(setenv_args_oldpwd) != 1 || shell_setenv(setenv_args_pwd) != 1)
	{
		fprintf(stderr, "cd: error updating environment variables\n");
		return (-1);
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

/* Liberar la memoria ocupada por el arreglo de argumentos */
void free_args(char *args[])
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	{
		free(args[i]); /* Liberar la memoria de cada argumento */
	}
}

