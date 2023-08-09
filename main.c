#include "shell.h"

int main(int argc, char *argv[])
{
	char *args[MAX_ARGS];

	if (argc == 1)
	{
		run_shell_loop();
	}
	else
	{
		/* // Modo no interactivo*/
		FILE *input_file = fopen(argv[1], "r");
		if (input_file)
		{
			char line[MAX_INPUT_LENGTH];
			while (fgets(line, sizeof(line), input_file))
			{
				printf("$ %s", line);
				tokenize_input(line, args);
			}
			fclose(input_file);
		}
		else
		{
			perror("Error opening input file");
			return (1);
		}
	}
	return (0);
}
