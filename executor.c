#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void execute_command(char *args[], int line_number)
{
	/* Obtener el valor de la variable de entorno PATH */
	char *path = _getenv("PATH");

	/* Duplicar la cadena de PATH para evitar modificaciones */
	char *path_copy = _strdup(path);

	/* Dividir la cadena PATH en directorios usando ":" como delimitador */
	char *dir = strtok(path_copy, ":");

	/* Crear un nuevo proceso hijo */
	pid_t pid = fork();

	/* Código dentro del proceso hijo */
	if (pid == 0)
	{
		/* Arreglo de variables de entorno para execve */
		char *env[] = {"LC_ALL=en_US.UTF-8", NULL};

		/* Verificar si el comando es ejecutable en la ubicación actual */
		if (access(args[0], X_OK) == 0)
		{
			/* Ejecutar el comando */
			execve(args[0], args, env);
			/* Mostrar mensaje de error si execve falla */
			perror("Error executing command");
			/* Salir del proceso hijo con un código de error */
			exit(EXIT_FAILURE);
		}

		/* Recorrer los directorios en PATH */
		while (dir != NULL)
		{
			/* Almacenar la ruta completa del ejecutable */
			char executable_path[MAX_INPUT_LENGTH];
			snprintf(executable_path, sizeof(executable_path), "%s/%s", dir, args[0]);

			/* Verificar si el comando es ejecutable en la nueva ruta */
			if (access(executable_path, X_OK) == 0)
			{
				/* Ejecutar el comando desde la nueva ruta */
				execve(executable_path, args, env);
				/* Mostrar mensaje de error si execve falla */
				perror("Error executing command");
				/* Salir del proceso hijo con un código de error */
				exit(EXIT_FAILURE);
			}

			/* Obtener el siguiente directorio en PATH */
			dir = strtok(NULL, ":");
		}

		/* Liberar la memoria de la copia de la cadena PATH */
		free(path_copy);

		/* Mostrar mensaje de comando no encontrado */
		fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, args[0]);
		/* Salir del proceso hijo con un código de error */
		exit(127);
	}
	/* Código en caso de fallo de fork */
	else if (pid < 0)
	{
		/* Mostrar mensaje de error de fork */
		print_error("Error forking");
	}
	/* Código dentro del proceso padre */
	else
	{
		/* Liberar la memoria de la copia de la cadena PATH */
		free(path_copy);
		/* Esperar a que el proceso hijo termine */
		wait(NULL);
	}
}
