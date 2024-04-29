NAME = minishell
HEADER = lexer.h
CC = cc
CFLAGS = -g -Wall -Wextra -Werror# -fsanitize=address

LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

SOURCES = env_main.c \
			env_utils.c \
			error_handling.c \
			exec_builtins_1.c \
			exec_builtins_2.c \
			exec_builtins_utils.c \
			execute_node_utils_exec_cmd.c \
			execute_node_utils_global.c \
			execute_node_utils_redirections.c \
			execute_node_utils.c \
			execute_parse_tree.c \
			execute_pipeline_utils.c \
			free_functions.c \
			lexer_main.c \
			lexer.c \
			main.c \
			parser_is_all_rest.c \
			parser_is_cmd_suffix.c \
			parser_main.c \
			parser_utils.c \

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