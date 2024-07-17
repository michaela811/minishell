NAME = minishell
HEADER = lexer.h
CC = cc
CFLAGS = -g -Wall -Wextra -Werror# -fsanitize=address

LIBDIR = ./libft
OBJDIR = ./obj
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
			init.c \
			custom_malloc.c \
			handle_quotes.c \
			handle_quotes_utils.c \
			handle_quotes_redirect.c \
			execute_pipeline_utils_1.c \
			exec_builtins_utils_1.c \
			exec_builtins_3.c \
			exec_builtins_4.c \
			exec_builtins_5.c \
			execude_node_utils_redirections_1.c \
			execute_node_utils_exec_utils.c \
			free_functions_1.c \
			free_functions_2.c \
			pipeline_here_doc.c \
			pipeline_here_doc_1.c \


OBJ = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(NAME): $(LIBFT) $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -lreadline -o $(NAME)

$(LIBFT):
		$(MAKE) -C $(LIBDIR) all

.PHONY: clean fclean all re

clean:
		$(MAKE) -C $(LIBDIR) clean
		rm -rf $(OBJDIR)

fclean: clean
		$(MAKE) -C $(LIBDIR) fclean
		rm -rf $(NAME)

re: fclean all