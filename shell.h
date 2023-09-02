#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64
#define MAX_LINES 100

extern char **environ;

int tokenize_input(char *input, char *args[]);
int execute_command(char *args[], int line_number, char *input);
int run_shell_loop(void);

int shell_cd(char *args[]);
int shell_exit(char *args[], int line_number, int child_status);
int shell_env(char *args[]);
int shell_setenv(char *args[]);
int shell_unsetenv(char *args[]);

void print_error(const char *message);

char *get_env_var(const char *name);
char *_getenv(const char *name);
char *_strdup(const char *str);
size_t _strlen(const char *str);
char *_strcpy(char *dest, const char *src);
int _strncmp(const char *s1, const char *s2, size_t n);
int _sstrcmp(const char *s1, const char *s2);
int _setenv(const char *name, const char *value);

char *read_input();
void free_args(char *args[]);
char *path_remover(char *arg);
char *getoptions(int argc, char *argv[]);
void malloc_environ();
int init_array_of_strings(char **array_of_strings, int size);
int init_string(char *string, int size);
long _strtol(const char *str, char **endptr, int base);
char *_strchr(const char *str, int c);

#endif /* SHELL_H */
