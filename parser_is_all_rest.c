/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_is_all_rest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:00 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 11:23:55 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cmd_word(t_token_list **tok, t_p_tree **cmd_word_node)
{
	if (*tok != NULL && (*tok)->token->type == WORD)
	{
		*cmd_word_node = alloc_parse_tree();
		if (*cmd_word_node == NULL)
			return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
		(*cmd_word_node)->data = (*tok)->token;
		*tok = (*tok)->next;
	}
	return (0);
}

static int	is_tok_null(t_token_list **tok)
{
	if (*tok == NULL || (*tok)->token == NULL)
		return (print_err(2, 2,
				"my(s)hell: syntax error near unexpected token `%s'\n",
				(*tok)->token->lexeme), 1);
	return (0);
}

static int	is_newline_error(t_token_list **tok, t_p_tree **redirect_node)
{
	if (*tok == NULL)
	{
		free_parse_tree(*redirect_node);
		print_err(2, 2,
			"my(s)hell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0);
}

int	is_io_redirect(t_token_list **tok, t_p_tree **redirect_node)
{
	if (is_tok_null(tok))
		return (1);
	if (is_redirection_token((*tok)->token->type))
	{
		*redirect_node = alloc_parse_tree();
		if (*redirect_node == NULL)
			return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
		(*redirect_node)->data = (*tok)->token;
		*tok = (*tok)->next;
		if (is_newline_error(tok, redirect_node))
			return (1);
		if ((*tok)->token->type != WORD || *tok == NULL)
			return (free_parse_tree(*redirect_node), print_err(2, 2,
					"my(s)hell: syntax error near unexpected token `%s'\n",
					(*tok)->token->lexeme), 1);
		(*redirect_node)->child = alloc_parse_tree();
		if ((*redirect_node)->child == NULL)
			return (free_parse_tree(*redirect_node), print_err(2, 2,
					"my(s)hell: memory error\n"), 1);
		(*redirect_node)->child->data = (*tok)->token;
		return ((*tok = (*tok)->next), 0);
	}
	return (print_err(2, 2,
			"my(s)hell: syntax error near unexpected token `%s'\n",
			(*tok)->token->lexeme), 1);
}

int	is_cmd_prefix(t_token_list **tok, t_p_tree **prefix_node)
{
	t_p_tree	*redirect_node;

	if (*tok != NULL && is_redirection_token((*tok)->token->type))
	{
		*prefix_node = alloc_parse_tree();
		if (prefix_node == NULL)
			return (print_err(1, 2, "my(s)hell: memory error\n"), 1);
	}
	while (*tok != NULL && is_redirection_token((*tok)->token->type))
	{
		redirect_node = NULL;
		if (is_io_redirect(tok, &redirect_node) != 0)
			return (free_parse_tree(*prefix_node), g_last_exit_status);
		link_node(prefix_node, redirect_node);
	}
	return (0);
}
