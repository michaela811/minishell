#include "lexer.h"

int	handle_redirection_token(t_token_list **tok, t_parse_tree **suffix_node)
{
	t_parse_tree	*redirect_node;

	redirect_node = NULL;
	if (is_io_redirect(tok, &redirect_node) != SUBTREE_OK)
		return (free_parse_tree(*suffix_node), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
	link_node(&(*suffix_node)->child, redirect_node);
	return (SUBTREE_OK);
}

int	handle_word_token(t_token_list **tok, t_parse_tree **suffix_node)
{
	t_parse_tree	*word_node;

	word_node = NULL;
	if (is_cmd_word(tok, &word_node) != SUBTREE_OK)
		return (free_parse_tree(*suffix_node), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
	link_node(&(*suffix_node)->child, word_node);
	return (SUBTREE_OK);
}

int	is_cmd_suffix(t_token_list **tok, t_parse_tree **suffix_node)
{
	if (*tok != NULL && ((*tok)->token->type == WORD
			|| is_redirection_token((*tok)->token->type)))
	{
		*suffix_node = alloc_parse_tree();
		if (*suffix_node == NULL)
			return (MEMORY_ERROR);
	}
	while (*tok != NULL && ((*tok)->token->type == WORD
			|| is_redirection_token((*tok)->token->type)))
	{
		if (is_redirection_token((*tok)->token->type))
		{
			if (handle_redirection_token(tok, suffix_node) != SUBTREE_OK)
				return (SYNTAX_ERROR);
		}
		else if ((*tok)->token->type == WORD)
		{
			if (handle_word_token(tok, suffix_node) != SUBTREE_OK)
				return (SYNTAX_ERROR);
		}
	}
	return (SUBTREE_OK);
}
