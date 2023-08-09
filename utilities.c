#include "shell.h"
#include <stdio.h>


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

if (n == 0) {
	return 0;
}

if (*s1) {
	return 1;
} else if (*s2) {
	return -1;
}

return 0;
}

size_t _strlen(const char *str)
{
	const char *s = str;
	while (*s)
	{
		s++;
	}
	return s - str;
}

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
	return dest;
}

char *_strdup(const char *str)
{
	size_t len = _strlen(str);
	char *copy = malloc(len + 1);
	if (copy) {
		_strcpy(copy, str);
	}
	return copy;
}

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
		return 1;
	}
	else if (*s2)
	{
		return -1;
	}

	return 0;
}


void print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}

char *_getenv(const char *name)
{
	extern char **environ;
	size_t name_len = _strlen(name);

	for (char **env = environ; *env != NULL; env++)
	{
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
		{
			return &(*env)[name_len + 1];
		}
	}
	return NULL;
}
