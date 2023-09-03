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

/* Prompt functions */
int run_shell_loop(void);
char *read_input();
int send_command(char *command, char *line, int status,
				 int line_number, int flag_logic_func);
void line_to_command(char *commands[], char *line, char *ptr_logic);
int tokenize_line(char *commands[], char *line, int *status);
void handle_logic(char *commands[], int logic_flag, char *line,
				  int line_number, int status);

/* Others */
void free_args(char *args[]);
void malloc_environ();
int init_array_of_strings(char **array_of_strings, int size);
int init_string(char *string, int size);
long _strtol(const char *str, char **endptr, int base);
char *_strchr(const char *str, int c);

#endif /* SHELL_H */
