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