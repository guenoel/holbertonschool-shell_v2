#include "shell.h"

/**
 * print_error - Prints an error message to the standard error output.
 * @message: Error message to print.
 */
void print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}

/**
 * _getenv - Gets the value of an environment variable.
 * @name: Name of the environment variable.
 *
 * Return: Pointer to the value of the environment variable or NULL if not
 * found.
 */

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

/**
 * _setenv - Sets an environment variable to a given value.
 * @name: Name of the environment variable.
 * @value: Value to assign to the environment variable.
 *
 * Return: 0 on success, -1 if memory allocation or setenv operation fails.
 */
int _setenv(const char *name, const char *value)
{
	char *new_var = NULL;
	int result = 0;

	if (_getenv(name) != NULL)
	{
		new_var = (char *)malloc(strlen(name) + strlen(value) + 2);
		if (new_var == NULL)
		{
			perror("_setenv");
			return (-1);
		}
		sprintf(new_var, "%s=%s", name, value);
		result = putenv(new_var);
		if (result != 0)
		{
			perror("_setenv");
			free(new_var); /* Liberar la memoria si no se pudo establecer la variable */
			return (-1);
		}
	} else
	{
		new_var = (char *)malloc(strlen(name) + strlen(value) + 2);
		if (new_var == NULL)
		{
			perror("_setenv");
			return (-1);
		}
		sprintf(new_var, "%s=%s", name, value);
		if (putenv(new_var) != 0)
		{
			perror("_setenv");
			free(new_var); /* Liberar la memoria si no se pudo establecer la variable */
			return (-1);
		}
	}
	return (0); /* Liberar la memoria si no se pudo establecer la variable */
}

/**
 * _strtol - Converts a string to a long integer.
 * @str: String to convert.
 * @endptr: Pointer to the first unconverted character (can be NULL).
 * @base: Conversion base (0 to guess base, 8 for octal, 10 for decimal,
 * 16 for hexadecimal).
 *
 * Return: The resulting long integer.
 */
long _strtol(const char *str, char **endptr, int base)
{
	long result = 0;
	bool is_negative = false;

	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '-' || *str == '+')
	{
		is_negative = (*str == '-');
		str++;
	}
	if (base == 0)
	{
		if (*str == '0')
		{
			base = (str[1] == 'x' || str[1] == 'X') ? 16 : 8;
			str += (base == 16) ? 2 : 1;
		} else
			base = 10;
	}
	while (*str != '\0')
	{
		char c = *str;
		int digit_value = -1;

		if ('0' <= c && c <= '9')
		{
			digit_value = c - '0';
		} else if (base == 16 && ('a' <= c && c <= 'f'))
		{
			digit_value = c - 'a' + 10;
		} else if (base == 16 && ('A' <= c && c <= 'F'))
			digit_value = c - 'A' + 10;
		if (digit_value == -1)
			break; /* Invalid character */
		result = result * base + digit_value;
		str++;
	}
		if (endptr != NULL)
			*endptr = (char *)str;
	return (is_negative ? -result : result);
}

/**
 * _strchr - Searches for the first occurrence of a character in a string.
 * @str: String to search within.
 * @c: Character to search for.
 *
 * Return: Pointer to the found character or NULL if not found.
 */
char *_strchr(const char *str, int c)
{
	if (str == NULL)
		return NULL;
	while (*str != '\0')
	{
		if (*str == c)
		{
			return ((char *)str);
		}
		str++;
	}

	if (c == '\0')
	{
		return ((char *)str);
	}

	return (NULL);
}
