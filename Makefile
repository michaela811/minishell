NAME = minishell
HEADER = lexer.h
CC = cc
CFLAGS = -g -Wall -Wextra -Werror# -fsanitize=address

LIBDIR = ./libft
OBJDIR = ./obj
LIBFT = $(LIBDIR)/libft.a

SOURCES = env_main.c \
			env_utils.c \
			env_handling.c \
			error_handling.c \
			exec_builtins_exit.c \
			exec_builtins_cd.c \
			exec_builtins_echo.c \
			exec_builtins_env.c \
			exec_builtins_export.c \
			exec_builtins_export_utils.c \
			exec_builtins_pwd.c \
			exec_builtins_unset.c \
			exec_builtins.c \
			execute_node.c \
			execute_node_handle_redirect.c \
			execute_node_handle_redirect_utils.c \
			execute_node_handle_redirect_error.c \
			execute_node_handle_heredoc.c \
			execute_node_handle_heredoc_utils.c \
			execute_node_exec_cmd.c \
			handle_dollar_sign.c \
			handle_dollar_sign_utils.c \
			execute_node_utils.c \
			execute_parse_tree.c \
			execute_pipeline_utils.c \
			free_functions.c \
			lexer_main.c \
			lexer.c \
			main.c \
			main_handle_input.c \
			parser_is_all_rest.c \
			parser_is_cmd_suffix.c \
			parser_main.c \
			parser_utils.c \
			init_1.c \
			init_2.c \
			handle_quotes_main_1.c \
			handle_quotes_main_2.c \
			handle_quotes_utils_1.c \
			handle_quotes_utils_2.c \
			handle_quotes_redirect.c \
			execute_pipeline_utils_1.c \
			execute_node_handle_child_cmd.c \
			execute_node_error_messages.c \
			free_functions_1.c \
			pipeline_here_doc_dol_closed.c \
			pipeline_here_doc_main.c \
			pipeline_here_doc_dol_open.c \
			get_path_for_exec.c \
			get_path_for_exec_utils.c \
			get_path_for_exec_colon.c \
			signals.c \


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