#include "minishell.h"

int	create_and_link_pipe(t_token_list **tok, t_p_tree **new)
{
	t_p_tree	*pipe_node;
	t_p_tree	*next_command;

	pipe_node = alloc_parse_tree();
	if (pipe_node == NULL)
		return (free_parse_tree(*new), print_err(1, 2,
				"my(s)hell: memory error\n"), 1);
	pipe_node->data = (*tok)->token;
	*tok = (*tok)->next;
	link_pipe(new, pipe_node);
	next_command = NULL;
	if (is_simple_command(tok, &next_command) != 0 || !next_command->child)
		return (g_last_exit_status);
	link_pipe(new, next_command);
	return (0);
}

void	print_token_list(t_token_list *list)
{
	t_token_list	*current;

	current = list;
	printf("my(s)hell: \'");
	while (current != NULL)
	{
		if (current->token != NULL)
		{
			if (current->next == NULL)
				printf("%s\'", current->token->lexeme);
			else
				printf("%s ", current->token->lexeme);
		}
		current = current->next;
	}
	printf("\n");
}

int	is_pipe_sequence(t_free_data *exec_data)
{
	t_p_tree	*current_command;

	if (exec_data->token_list == NULL || exec_data->token_list->token == NULL)
		return (print_err(2, 2, "my(s)hell: syntax error\n"), 1);
	exec_data->tree = alloc_parse_tree();
	exec_data->tree_start = exec_data->tree;
	if (exec_data->tree == NULL)
		return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
	current_command = NULL;
	if (is_simple_command(&exec_data->token_list, &current_command) != 0
		|| !current_command->child)
		return (free_command_data(exec_data), g_last_exit_status);
	exec_data->tree->child = current_command;
	while (exec_data->token_list != NULL
		&& exec_data->token_list->token->type == PIPE)
	{
		if (create_and_link_pipe(&exec_data->token_list, &exec_data->tree) != 0)
			return (free_command_data(exec_data), g_last_exit_status);
	}
	return (0);
}

int	is_simple_command(t_token_list **tok, t_p_tree **new)
{
	t_p_tree	*cmd_word_node;
	t_p_tree	*cmd_suffix_node;

	*new = alloc_parse_tree();
	if (*new == NULL)
		return (print_err(2, 2,
				"my(s)hell: syntax error near unexpected token `%s'\n",
				(*tok)->token->lexeme), 1);
	if (is_cmd_prefix(tok, &((*new)->child)) != 0)
		return (free(*new), g_last_exit_status);
	cmd_word_node = NULL;
	if (is_cmd_word(tok, &cmd_word_node) != 0)
		return (free_parse_tree(*new), g_last_exit_status);
	if (cmd_word_node == NULL && (*new)->child)
		return (0);
	if (cmd_word_node == NULL && !(*new)->child)
		return (free_parse_tree(*new), print_err(2, 2,
				"my(s)hell: syntax error near unexpected token `%s'\n",
				(*tok)->token->lexeme), 1);
	link_node(&((*new)->child), cmd_word_node);
	cmd_suffix_node = NULL;
	if (is_cmd_suffix(tok, &cmd_suffix_node) == 0)
	{
		if (cmd_suffix_node && cmd_suffix_node->child != NULL)
			link_node(&((*new)->child), cmd_suffix_node);
	}
	else
		return (free_parse_tree(*new), g_last_exit_status);
	return (0);
}

bool	is_redirection_token(enum e_token_type type)
{
	if (type == RED_FROM || type == RED_TO
		|| type == APPEND || type == HERE_DOC)
		return (true);
	return (false);
}
