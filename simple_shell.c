#include "shell.h"

/**
 * main - main function
 *
 * Return: End status
 */

int main(void) {
  ssize_t nread;
  size_t len = 0;
  int status = 0;
  pid_t child;
  char *del = " \n";
  char *buf = NULL;
  char **args = (char **)calloc(1, sizeof(char *));
	int i;

  while ((nread = getline(&buf, &len, stdin)) != -1) {
    args = split(args, buf, del);
    if (args == NULL || args[0] == NULL)
      continue;
		if (args)
		{
			// Si hay al menos un argumento
			if (fork() == 0)
			{
					// Proceso hijo: ejecuta el comando con execvp
					execve(args[0], args, environ);
					exit(0); // Termina el proceso hijo
			}
			else
					wait(NULL); // Espera a que el proceso hijo termine
		}
    /*path_handler(args);*/
    child = fork();
    if (child == -1) {
      perror("Error:");
      return (1);
    }
    if (child == 0) {
      if (args[0] && execve(args[0], args, environ) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
      }
      free(args);
      free_memory(buf);
    } else
      wait(&status);
  }
  free(args);
  free_memory(buf);
  return (0);
}

/**
* exit_fun - Builtin function that exits from shell
* Return: 0
*/
int exit_fun(void)
{
	exit(0);
}