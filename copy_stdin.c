#include "shell.h"

int copy_stdin(void) {
	/* Open the file in write mode */
	int file_fd = open("stdin.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file_fd == -1) {
		perror("Error opening file");
		return 1;
	}

	char buffer[4096];
	ssize_t bytes_read;

	/* Read input from stdin and write it to the file */
	while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		ssize_t bytes_written = write(file_fd, buffer, bytes_read);
		if (bytes_written == -1) {
			perror("Error writing to file");
			close(file_fd);
			return 1;
		}
	}

	if (bytes_read == -1) {
		perror("Error reading from stdin");
		close(file_fd);
		return 1;
	}

	/* Close the file */
	if (close(file_fd) == -1) {
		perror("Error closing file");
		return 1;
	}

	return 0;
}