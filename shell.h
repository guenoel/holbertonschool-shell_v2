#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <stddef.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

extern char **environ;

int tokenize_input(char *input, char *args[]);
void execute_command(char *args[], int line_number);
void run_shell_loop(void);

int shell_cd(char *args[]);
int shell_exit(char *args[]);
int shell_env(char *args[]);
int shell_setenv(char *args[]);
int shell_unsetenv(char *args[]);

void print_error(const char *message);

char *_getenv(const char *name);
char *_strdup(const char *str);
size_t _strlen(const char *str);
char *_strcpy(char *dest, const char *src);
int _strncmp(const char *s1, const char *s2, size_t n);
int _sstrcmp(const char *s1, const char *s2);

char *read_input();
void free_args(char *args[]);
int is_eof();

#endif /* SHELL_H */