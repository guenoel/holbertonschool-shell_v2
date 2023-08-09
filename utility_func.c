#include "shell.h"

/**
 * free_array_memory - free pointers of pointers
 * @array: pointer of pointers
 *
 */

void free_array_memory(char **array) {
  int i;

  if (array) {
    for (i = 0; array[i]; i++) {
      if (array[i]) {
        free(array[i]);
        array[i] = NULL;
      }
    }
    free(array);
    array = NULL;
  }
}

/**
 * free_memory - free pointer
 * @pointer: pointer
 *
 */

void free_memory(char *pointer) {
  if (pointer) {
    free(pointer);
    pointer = NULL;
  }
}

/**
 * split - function to split line buffer to command + arguments
 * @buf: buffer
 * @del: delimiters
 *
 * Return: array full of pointers to command + arguments
 */

char **split(char **args, char *buf, char *del) {
  int i = 0;
  char *token;

  token = strtok(buf, del);
  while (token) {
    /*printf("[%s]\n", token);*/
    args = (char **)realloc(args, (i + 2) * sizeof(char *));
    args[i] = token;
    token = strtok(NULL, del);
    i++;
  }
  args[i] = NULL;
  return args;
}
