CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEPS = lexer.h parser.h
OBJ = lexer.o parser.o main.o

%.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

program: $(OBJ)
    $(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
    rm -f $(OBJ) program