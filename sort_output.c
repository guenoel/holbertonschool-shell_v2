#include "shell.h"

/* Custom comparison function for qsort */
int compare_strings(const void *a, const void *b) {
	const char *str1 = *(const char **)a;
	const char *str2 = *(const char **)b;
	return strcasecmp(str1, str2);
}

/**
 * is_char_in_str - fonction principale
 * @str: string to check
 * @c: char to compare
 *
 * Return: true if contained, false if not
 */

int is_char_in_str(const char *str, char c)
{
	if (str == NULL) {
		return (0);
	}

	while (*str != '\0') {
		if (*str == c) {
			return (1);
		}
		str++;
	}

	return (0);
}

int print_sorted_output(char *executable_path, char *options, char *args[], char **env)
{
	FILE *cmd_output = NULL;
	/* Have to adjust size of lines if necessary */
	char *lines[MAX_LINES];
	char buffer[1024];
	size_t num_lines = 0;
	size_t i = 0;

	pid_t pid = fork();
	if (pid == 0) {
		/* Child process */
		cmd_output = fopen("cmd_output.txt", "w");
		if (cmd_output == NULL) {
			perror("fopen");
			_exit(1);
		}
		/* Redirect stdout to the file */
		dup2(fileno(cmd_output), STDOUT_FILENO);
		execve(executable_path, args, env);
		perror("execve");
		_exit(1);
	} else if (pid > 0) {
		/* Parent process */
		int status;
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			cmd_output = fopen("cmd_output.txt", "r");
			if (cmd_output == NULL) {
				perror("fopen");
				return 1;
			}
		} else {
			printf("Command execution failed.\n");
			return 1;
		}
	} else {
		perror("fork");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), cmd_output)) {
		/* Remove trailing newline */

		if (num_lines < MAX_LINES)
		{
            buffer[strcspn(buffer, "\n")] = '\0';
			lines[num_lines] = strdup(buffer);
			num_lines++;
        } else {
            break;
        }
	}

	fclose(cmd_output);

	qsort(lines, num_lines, sizeof(char *), compare_strings);

	for (i = 0; i < num_lines; i++)
	{
		printf("%s", lines[i]);
		if(is_char_in_str(options, '1') || is_char_in_str(options, 'l'))
		{
			printf("\n");
		} else {
			printf(" ");
		}
		free(lines[i]);
	}
	if (!is_char_in_str(options, '1') && !is_char_in_str(options, 'l'))
	{
		printf("\n");
	}

	return (0);
}