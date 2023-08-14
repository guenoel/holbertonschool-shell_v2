#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

extern char **environ;

typedef struct Env_Node
{
    char *value;
    struct Env_Node *next;
} Env_t;

typedef struct Env_List
{
    char *name;
    struct Env_List *next;
    Env_t *list;
} Env_l_t;

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
int compare_data(const void *a, const void *b);

char *read_input();
void free_args(char *args[]);

Env_t *add_node(Env_t *list, char *value);
void free_list(struct Env_Node *list);
void free_list_list(Env_l_t *list_env);
Env_l_t *load_env(char **environ);
Env_l_t *add_list(Env_l_t *lists, char *name, Env_t *list);

#endif

