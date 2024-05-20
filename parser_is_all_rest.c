#include "minishell.h"

int	is_cmd_word(t_token_list **tok, t_parse_tree **cmd_word_node)
{
	if (*tok != NULL && (*tok)->token->type == WORD)
	{
		*cmd_word_node = alloc_parse_tree();
		if (*cmd_word_node == NULL)
			return (printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
		(*cmd_word_node)->data = (*tok)->token;
		*tok = (*tok)->next;
	}
	return(0);
}

int	is_io_redirect(t_token_list **tok, t_parse_tree **redirect_node)
{
	if (*tok == NULL || (*tok)->token == NULL)
		return (printf_global_error(1, 2, "my(s)hell: syntax error\n"), 1);
	if (is_redirection_token((*tok)->token->type))
	{
		*redirect_node = alloc_parse_tree();
		if (*redirect_node == NULL)
			return (printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
		(*redirect_node)->data = (*tok)->token;
		*tok = (*tok)->next;
		if (*tok == NULL || (*tok)->token->type != WORD)
			return (free_parse_tree(*redirect_node), printf_global_error(1, 2, "my(s)hell: syntax error\n"), 1);
		(*redirect_node)->child = alloc_parse_tree();
		if ((*redirect_node)->child == NULL)
			return (free_parse_tree(*redirect_node), printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
		(*redirect_node)->child->data = (*tok)->token;
		*tok = (*tok)->next;
		return (0);
	}
	return (printf_global_error(1, 2, "my(s)hell: syntax error\n"), 1);
}

bool	is_redirection_token(enum token_type type)
{
	if (type == RED_FROM || type == RED_TO
		|| type == APPEND || type == HERE_DOC)
		return (true);
	return (false);
}

int	is_cmd_prefix(t_token_list **tok, t_parse_tree **prefix_node)
{
	t_parse_tree	*redirect_node;

	if (*tok != NULL && is_redirection_token((*tok)->token->type))
	{
		*prefix_node = alloc_parse_tree();
		if (prefix_node == NULL)
			return (printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
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
