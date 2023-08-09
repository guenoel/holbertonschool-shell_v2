#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
extern char **environ;

void free_array_memory(char **array);
void free_memory(char *pointer);
char **mystrtok(char *buf, char **args);
char **split(char **args, char *buf, char *del);
void path_handler(char **args);
int exit_fun(void);

#endif
