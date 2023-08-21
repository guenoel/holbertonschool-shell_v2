#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

char *path_remover(char *arg)
{
	char *prog = NULL;
	char *arg_copy = _strdup(arg);
	char *token = strtok(arg_copy, "/");

	while (token != NULL)
	{
		token = strtok(NULL, "/");
		if (token)
		{
			free(prog);
			prog = _strdup(token);
		}
	}
	free(arg_copy);
	return prog;
}

/* Function to copy the environ array into a new variable */
char **copy_environ() {
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

    return new_env;
}

void execute_command(char *args[], int line_number)
{
	char *dir = NULL;
	char *path = NULL;
	char *path_copy = NULL;
	pid_t pid = 0;

	/* Obtener el valor de la variable de entorno PATH */
	path = _getenv("PATH");

	/* Duplicar la cadena de PATH para evitar modificaciones */
	path_copy = _strdup(path);

	/* Dividir la cadena PATH en directorios usando ":" como delimitador */
	dir = strtok(path_copy, ":");

	/* Crear un nuevo proceso hijo */
	pid = fork();

	/* Código dentro del proceso hijo */
	if (pid == 0)
	{
		/* Arreglo de variables de entorno para execve */
		char **env = environ;

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
/* 				if (_sstrcmp(args[0], "ls") == 0)
				{
					char executable_ls[MAX_INPUT_LENGTH * 2];
					snprintf(executable_ls, sizeof(executable_ls), "%s%s", "LC_ALL=en_US.UTF-8 ", executable_path);
					printf("executable_ls: %s\n", executable_ls);
					execve(executable_ls, args, environ);
				} else
				{ */
					/* Ejecutar el comando desde la nueva ruta */
				execve(executable_path, args, env);
				/* } */
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
