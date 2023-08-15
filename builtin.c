#include "shell.h"
#include <stdlib.h>

#define UNUSED(x) (void)(x)

/* Cambiar el directorio actual */
int shell_cd(char *args[])
{
	if (args[1] == NULL)
	{
		chdir(_getenv("HOME")); /* Cambiar al directorio HOME si no se proporciona argumento */
	}
	else
	{
		if (chdir(args[1]) != 0) /* Cambiar al directorio especificado por el argumento */
		{
			perror("cd"); /* Mostrar mensaje de error si chdir falla */
		}
	}
	return (1); /* Indicar que el comando se ejecutó correctamente */
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
		if (setenv(args[1], args[2], 1) != 0) /* Establecer la variable de entorno con el valor proporcionado */
		{
			perror("setenv"); /* Mostrar mensaje de error si setenv falla */
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
