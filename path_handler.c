#include "shell.h"

/**
 * path_handler - add path to command
 * @args: list of arguments (args[0] is the command)
 */

void path_handler(char **args) {
  int i;
  char *path = strdup(getenv("PATH"));
  char *mem = NULL;
  char **path_array = NULL;

  /** if it doesn't exist, alors on s'arrete là*/
  if (path == NULL)
    return;

  path_array = split(args, path, ":");
  free_memory(path);
  /** Check if the command written is present in one of the path */
  for (i = 0; path_array[i]; i++) {
    /** verif si malloc dans les declarations s'est bien passé*/
    mem =
        (char *)calloc(sizeof(char), (strlen(path_array[i]) + 1 + strlen(args[0]) + 1));
    if (mem == NULL)
      break;
    /** Concatenate the path before the command */
    strcat(mem, path_array[i]);
    strcat(mem, "/");
    strcat(mem, args[0]);
    /** Check if it can acces the command*/
    /* with the path and if not free all */
    if (access(mem, F_OK) == 0)
      break;
    free_memory(mem);
  }
  free_memory(args[0]);
  free_array_memory(path_array);
  /** Redefine the command pointer to the adress*/
  /* of the command with the path in it so the*/
  /* execve can execute the command properly */
  args[0] = mem;
}
