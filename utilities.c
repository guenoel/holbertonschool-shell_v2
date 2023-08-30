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
	if (str == NULL)
		return (NULL);
	else
	{
		size_t len = _strlen(str);
		char *copy = (char *)malloc(len + 1);
			if (copy)
			{
				_strcpy(copy, str);
			}
		return (copy);
	}
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
	char *new_var = NULL;
	int result = 0;
	/* Verificar si la variable de entorno ya existe */
	if (_getenv(name) != NULL)
	{
		/* Crear una nueva cadena en memoria y asignarle el valor */
		new_var = (char *)malloc(strlen(name) + strlen(value) + 2); /* +2 para el igual y el terminador nulo */
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
		new_var = (char *)malloc(strlen(name) + strlen(value) + 2); /* +2 para el igual y el terminador nulo */
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

long _strtol(const char *str, char **endptr, int base) {
    long result = 0;
    bool is_negative = false;

    /* Skip leading white spaces */
    while (*str == ' ' || *str == '\t') {
        str++;
    }

    /* Handle sign */
    if (*str == '-') {
        is_negative = true;
        str++;
    } else if (*str == '+') {
        str++;
    }

    /* Handle base prefix (0x for hexadecimal, 0 for octal) */
    if (base == 0) {
        if (*str == '0') {
            str++;
            if (*str == 'x' || *str == 'X') {
                base = 16;
                str++;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }

    /* Conversion loop */
    while (*str != '\0') {
        char c = *str;
        int digit_value = -1;

        if ('0' <= c && c <= '9') {
            digit_value = c - '0';
        } else if (base == 16 && ('a' <= c && c <= 'f')) {
            digit_value = c - 'a' + 10;
        } else if (base == 16 && ('A' <= c && c <= 'F')) {
            digit_value = c - 'A' + 10;
        }

        if (digit_value == -1) {
            break; /* Invalid character */
        }

        result = result * base + digit_value;
        str++;
    }

    /* Set endptr to point to the first non-convertible character */
    if (endptr != NULL) {
        *endptr = (char *)str;
    }

    return is_negative ? -result : result;
}

char *_strchr(const char *str, int c) {
	if (str == NULL)
		return NULL;
    while (*str != '\0') {
        if (*str == c) {
            return (char *)str;
        }
        str++;
    }
    
    if (c == '\0') {
        return (char *)str;
    }

    return NULL;
}