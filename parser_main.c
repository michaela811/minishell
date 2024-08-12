/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:11 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/12 11:08:33 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_and_link_pipe(t_token_list **tok, t_p_tree **new)
{
	t_p_tree		*pipe_node;
	t_p_tree		*next_command;
	t_token_list	*tok_prev;

	pipe_node = alloc_parse_tree();
	if (pipe_node == NULL)
		return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
	pipe_node->data = (*tok)->token;
	tok_prev = *tok;
	*tok = (*tok)->next;
	link_pipe(new, pipe_node);
	next_command = NULL;
	if (is_simple_command(tok, &next_command, tok_prev) != 0
		|| !next_command->child)
		return (g_last_exit_status);
	link_pipe(new, next_command);
	return (0);
}

int	is_pipe_sequence(t_free_data *exec_data)
{
	t_p_tree	*current_command;

	if (exec_data->token_list == NULL || exec_data->token_list->token == NULL)
		return (print_err(2, 2, "my(s)hell: syntax error\n"), 1);
	exec_data->tree = alloc_parse_tree();
	if (exec_data->tree == NULL)
		return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
	exec_data->tree_start = exec_data->tree;
	if (exec_data->tree == NULL)
		return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
	current_command = NULL;
	if (is_simple_command(&exec_data->token_list, &current_command,
			exec_data->token_list) != 0)
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

static int	is_new_null(t_p_tree **new)
{
	if (*new == NULL)
		return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
	return (0);
}

static int	is_it_syntax_error(t_p_tree	*cmd_word_node, t_p_tree **new,
		t_token_list *tok_prev)
{
	if (cmd_word_node == NULL && !(*new)->child)
		return (free_parse_tree(*new), print_err(2, 2,
				"my(s)hell: syntax error near unexpected token `%s'\n",
				tok_prev->token->lexeme), 1);
	return (0);
}

int	is_simple_command(t_token_list **tok, t_p_tree **new,
		t_token_list *tok_prev)
{
	t_p_tree	*cmd_word_node;
	t_p_tree	*cmd_suffix_node;

	*new = alloc_parse_tree();
	if (is_new_null(new))
		return (1);
	if (is_cmd_prefix(tok, &((*new)->child)) != 0)
		return (free_parse_tree(*new), g_last_exit_status);
	cmd_word_node = NULL;
	if (is_cmd_word(tok, &cmd_word_node) != 0)
		return (free_parse_tree(*new), g_last_exit_status);
	if (cmd_word_node == NULL && (*new)->child)
		return (0);
	if (is_it_syntax_error(cmd_word_node, new, tok_prev))
		return (1);
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
