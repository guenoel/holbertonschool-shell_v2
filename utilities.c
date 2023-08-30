#include "shell.h"

/**
 * _strlen - computes lenght of string
 * @str: pointer to string
 * Return: int len of string
*/
size_t _strlen(const char *str)
{
	const char *s = str;
		while (*s)
		{
			s++;
		}
		return (s - str);
	}

/**
 * _strncmp - Compares two strings up to n characters.
 * @s1: First string to compare.
 * @s2: Second string to compare.
 * @n: Number of characters to compare.
 *
 * Return: 0 if the strings are equal up to n characters,
 * a negative value if s1 < s2, or a positive value if s1 > s2.
 */
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

/**
 * _strcpy - Copies a source string to a destination string.
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 *
 * Return: Pointer to the destination string.
 */
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

/**
 * _strdup - Duplicates a string.
 * @str: Pointer to the string to duplicate.
 *
 * Return: Pointer to a new copy of the string, or NULL if memory allocation
 * fails.
 */
char *_strdup(const char *str)
{
	size_t len;
	char *copy;

	if (str == NULL)
		return (NULL);

	len = _strlen(str);
	copy = (char *)malloc(len + 1);
		if (copy)
		{
			_strcpy(copy, str);
		}
	return (copy);
}

/**
 * _sstrcmp - Compares two strings.
 * @s1: First string to compare.
 * @s2: Second string to compare.
 *
 * Return: 0 if the strings are equal, a negative value if s1 < s2,
 * or a positive value if s1 > s2.
 */
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
