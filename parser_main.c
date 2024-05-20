#include "minishell.h"

int	create_and_link_pipe(t_token_list **tok, t_parse_tree **new)
		//t_parse_tree *pipe_node, t_parse_tree *next_command)
{
	t_parse_tree	*pipe_node;
	t_parse_tree	*next_command;

	pipe_node = alloc_parse_tree();
	if (pipe_node == NULL)
		return (free_parse_tree(*new), printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
	pipe_node->data = (*tok)->token;
	*tok = (*tok)->next;
	link_pipe(new, pipe_node);
	next_command = NULL;
	if (is_simple_command(tok, &next_command) != 0 || !next_command->child)
		return (free_parse_tree(*new), g_last_exit_status);
	link_pipe(new, next_command);
	return (0);
}

int	is_pipe_sequence(t_free_data *free_data)
{
	t_parse_tree	*current_command;
	//t_parse_tree	*pipe_node;
	//t_parse_tree	*next_command;

	if (free_data->token_list == NULL || free_data->token_list->token == NULL)
		return (printf_global_error(1, 2, "my(s)hell: syntax error\n"), 1);
	free_data->tree = alloc_parse_tree();
	if (free_data->tree == NULL)
		return (printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
	current_command = NULL;
	if (is_simple_command(&free_data->token_list, &current_command) != 0
		|| !current_command->child)
		return (free_parse_tree(free_data->tree), g_last_exit_status);
	free_data->tree->child = current_command;
	while (free_data->token_list != NULL && free_data->token_list->token->type == PIPE)
	{
		if (create_and_link_pipe(&free_data->token_list, &free_data->tree) != 0)
			return (free_command_data(free_data), g_last_exit_status);
	}
	//free_token_list(*tok);
	return (0);
}

int	is_simple_command(t_token_list **tok, t_parse_tree **new)
{
	t_parse_tree	*cmd_word_node;
	t_parse_tree	*cmd_suffix_node;

	*new = alloc_parse_tree();
	if (*new == NULL)
		return (printf_global_error(1, 2, "my(s)hell: syntax error\n"), 1);
	if (is_cmd_prefix(tok, &((*new)->child)) != 0)
		return (free_parse_tree(*new), g_last_exit_status);
	cmd_word_node = NULL;
	if (is_cmd_word(tok, &cmd_word_node) != 0)
		return (free_parse_tree(*new), g_last_exit_status);
	if (cmd_word_node == NULL)// PROBABLY CHECK WHETHER THE TOKEN LIST IS EMPTY
		return (0);
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
