#include "minishell.h"

int	handle_redirection_token(t_token_list **tok, t_parse_tree **suffix_node)
{
	t_parse_tree	*redirect_node;

	redirect_node = NULL;
	if (is_io_redirect(tok, &redirect_node) != 0)
		return (free_parse_tree(*suffix_node), g_last_exit_status);
	link_node(&(*suffix_node)->child, redirect_node);
	return (0);
}

int	handle_word_token(t_token_list **tok, t_parse_tree **suffix_node)
{
	t_parse_tree	*word_node;

	word_node = NULL;
	if (is_cmd_word(tok, &word_node) != 0)
		return (free_parse_tree(*suffix_node), g_last_exit_status);
	link_node(&(*suffix_node)->child, word_node);
	return (0);
}

int	is_cmd_suffix(t_token_list **tok, t_parse_tree **suffix_node)
{
	if (*tok != NULL && ((*tok)->token->type == WORD
			|| is_redirection_token((*tok)->token->type)))
	{
		*suffix_node = alloc_parse_tree();
		if (*suffix_node == NULL)
			return (printf_global_error(1, 2, "my(s)hell: memory error\n"), 1);
	}
	while (*tok != NULL && ((*tok)->token->type == WORD
			|| is_redirection_token((*tok)->token->type)))
	{
		if (is_redirection_token((*tok)->token->type))
		{
			if (handle_redirection_token(tok, suffix_node) != 0)
				return (g_last_exit_status);
		}
		else if ((*tok)->token->type == WORD)
		{
			if (handle_word_token(tok, suffix_node) != 0)
				return (g_last_exit_status);
		}
	}
	return (0);
}
