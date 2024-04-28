NAME = minishell
HEADER = lexer.h
CC = cc
CFLAGS = -g #-WALL -Wextra -Werror -fsanitize=address

LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

SOURCES = execute_all.c \
			lexer.c \
			main.c \
			parser.c \
			error_handling.c \
			free_functions.c \
			main_envp.c \

OBJ = $(SOURCES:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(NAME): $(LIBFT) $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -lreadline -o $(NAME)

$(LIBFT):
		$(MAKE) -C $(LIBDIR) all

.PHONY: clean fclean all re

clean:
		$(MAKE) -C $(LIBDIR) clean
		rm -f $(OBJ)

fclean: clean
		$(MAKE) -C $(LIBDIR) fclean
		rm -rf $(NAME)

re: fclean all