#include "shell.h"
#include <stdio.h>

/* Comparar dos cadenas hasta n caracteres */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 && *s2)
	{
		if (*s1 != *s2)
		{
			return (*s1 - *s2);
		}
		s1++;
		s2++;
		n--;
	}

	if (n == 0)
	{
		return (0);
	}

	if (*s1)
	{
		return (1);
	}
	else if (*s2)
	{
		return (-1);
	}

	return (0);
}

/* Calcular la longitud de una cadena */
size_t _strlen(const char *str)
{
	const char *s = str;
	while (*s)
	{
		s++;
	}
	return (s - str);
}

/* Copiar una cadena de origen a un destino */
char *_strcpy(char *dest, const char *src)
{
	char *d = dest;
	while (*src)
	{
		*d = *src;
		d++;
		src++;
	}
	*d = '\0';
	return (dest);
}

/* Duplicar una cadena */
char *_strdup(const char *str)
{
	size_t len = _strlen(str);
	char *copy = (char *)malloc(len + 1);
	if (copy)
	{
		_strcpy(copy, str);
	}
	return (copy);
}

/* Comparar dos cadenas */
int _sstrcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
		{
			return (*s1 - *s2);
		}
		s1++;
		s2++;
	}

	if (*s1)
	{
		return (1);
	}
	else if (*s2)
	{
		return (-1);
	}

	return (0);
}

/* Imprimir un mensaje de error */
void print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}

/* Obtener el valor de una variable de entorno */
char *_getenv(const char *name)
{
	char **env = NULL;
	size_t name_len = _strlen(name);

	for (env = environ; *env != NULL; env++)
	{
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
		{
			return (&(*env)[name_len + 1]);
		}
	}
	return (NULL);
}

int _setenv(const char *name, const char *value)
{
	int result = 0;
	/* Verificar si la variable de entorno ya existe */
	if (_getenv(name) != NULL)
	{
		/* Crear una nueva cadena en memoria y asignarle el valor */
		char *new_var = malloc(strlen(name) + strlen(value) + 2); /* +2 para el igual y el terminador nulo */
		if (new_var == NULL)
		{
			perror("_setenv");
			return -1;
		}

		sprintf(new_var, "%s=%s", name, value);

		/* Reemplazar la variable de entorno */
		result = putenv(new_var);
		if (result != 0)
		{
			perror("_setenv");
			free(new_var); /* Liberar la memoria si no se pudo establecer la variable */
			return -1;
		}
	}
	else
	{
		/* Crear una nueva cadena en memoria y asignarle el nombre y el valor */
		char *new_var = malloc(strlen(name) + strlen(value) + 2); /* +2 para el igual y el terminador nulo */
		if (new_var == NULL)
		{
			perror("_setenv");
			return -1;
		}

		sprintf(new_var, "%s=%s", name, value);

		/* Agregar la nueva variable al arreglo de variables de entorno */
		if (putenv(new_var) != 0)
		{
			perror("_setenv");
			free(new_var); /* Liberar la memoria si no se pudo establecer la variable */
			return -1;
		}
	}

	return 0; /* Liberar la memoria si no se pudo establecer la variable */
}
