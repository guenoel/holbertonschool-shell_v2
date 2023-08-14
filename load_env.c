#include "shell.h"

Env_t *add_node(Env_t *list, char *value)
{
	Env_t *new_node = (Env_t *)malloc(sizeof(Env_t));
	if (new_node != NULL) {
		new_node->value = strdup(value);
		// Store the value separated by ';' in the same Env_Node
		new_node->next = list;
		list = new_node;
	}
	return list;
}

Env_l_t *add_list(Env_l_t *lists, char *name, Env_t *list)
{
	Env_l_t *new_node = (Env_l_t *)malloc(sizeof(Env_l_t));
	if (new_node != NULL) {
		new_node->name = strdup(name);
		new_node->list = list;
		new_node->next = lists;
		lists = new_node;
	}
	return lists;
}

void free_list(Env_t *list)
{
	while (list) {
		Env_t *temp = list;
		list = list->next;
		free(temp->value);
		free(temp);
	}
}

void free_list_list(Env_l_t *list_env)
{while (list_env)
	{
		Env_l_t *temp = list_env;
		list_env = list_env->next;
		free_list(temp->list);
		free(temp->name);
		free(temp);
	}
}

Env_l_t *load_env(char **environ)
{
	Env_l_t *env_lists = NULL;

	char **env = environ;
    while (*env) {
		Env_t *env_list = NULL;

		char *name = strtok(*env, "=");
		if (name != NULL) {
			char *value = strtok(NULL, ":");
            while (value != NULL) {
				env_list = add_node(env_list, value);
				value = strtok(NULL, ":");
			}
		}
		env_lists = add_list(env_lists, name, env_list);

		env++;
	}

	return env_lists;
}
