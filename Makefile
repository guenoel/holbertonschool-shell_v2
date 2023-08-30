CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
SRCS = main.c prompt.c tokenizer.c executor.c builtin.c utilities.c utilities1.c functions_env.c
OBJS = $(SRCS:.c=.o)
TARGET = hsh

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)
