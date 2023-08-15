#include "shell.h"
#include <stdlib.h>

#define UNUSED(x) (void)(x)

int shell_cd(char *args[])
{
	char *old_pwd = _getenv("PWD"); /* Obtener el valor actual de PWD */
	char new_pwd[MAX_INPUT_LENGTH];
	/* Si no se proporciona ningún argumento, cambia al directorio HOME */
	if (args[1] == NULL)
	{
		char *home_dir = _getenv("HOME");
			if (chdir(home_dir) == 0)
			{
				/* Actualizar las variables de entorno */
				_setenv("OLDPWD", old_pwd);
				_setenv("PWD", home_dir);
				return (0);
			}
			else
			{
				perror("cd");
				return (-1);
			}
	}
	/* Si el argumento comienza con "-", muestra el directorio actual o el valor de OLDPWD */
	else if (args[1][0] == '-')
	{
		char *prev_dir = _getenv("OLDPWD");
			if (prev_dir == NULL)
			{
				fprintf(stderr, "No se ha establecido OLDPWD\n");
				return (-1);
			}
		printf("%s\n", prev_dir); /* Muestra el valor de OLDPWD */
		if (chdir(prev_dir) == 0)
		{
			/* Actualizar las variables de entorno */
			_setenv("OLDPWD", old_pwd);
			_setenv("PWD", prev_dir);
			return (0);
		}
		else
		{
			perror("cd");
			return (-1);
		}
	}
	/* En otros casos, cambia al directorio especificado en args[1] */
	else
	{
		if (chdir(args[1]) == 0)
		{
			/* Actualizar las variables de entorno */
			_setenv("OLDPWD", old_pwd);
			getcwd(new_pwd, sizeof(new_pwd));
			_setenv("PWD", new_pwd);
			return (0);
		}
		else
		{
			fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", args[1]);
			return (-1);
		}
	}
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
	if (args[1] != NULL && args[2] != NULL)
	{
		if (_setenv(args[1], args[2]) != 0) /* Establecer la variable de entorno con el valor proporcionado */
		{
			perror("_setenv"); /* Mostrar mensaje de error si setenv falla */
		}
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
