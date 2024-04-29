#include "lexer.h"

int	is_cmd_word(t_token_list **tok, t_parse_tree **cmd_word_node)
{
	if (*tok != NULL && (*tok)->token->type == WORD)
	{
		*cmd_word_node = alloc_parse_tree();
		if (*cmd_word_node == NULL)
			return (MEMORY_ERROR);
		(*cmd_word_node)->data = (*tok)->token;
		*tok = (*tok)->next;
	}
	return (SUBTREE_OK);
}

int	is_io_redirect(t_token_list **tok, t_parse_tree **redirect_node)
{
	if (*tok == NULL || (*tok)->token == NULL)
		return (SYNTAX_ERROR);
	if (is_redirection_token((*tok)->token->type))
	{
		*redirect_node = alloc_parse_tree();
		if (*redirect_node == NULL)
			return (MEMORY_ERROR);
		(*redirect_node)->data = (*tok)->token;
		*tok = (*tok)->next;
		if (*tok == NULL || (*tok)->token->type != WORD)
			return (free_parse_tree(*redirect_node), SYNTAX_ERROR);
		(*redirect_node)->child = alloc_parse_tree();
		if ((*redirect_node)->child == NULL)
			return (free_parse_tree(*redirect_node), MEMORY_ERROR);
		(*redirect_node)->child->data = (*tok)->token;
		*tok = (*tok)->next;
		return (SUBTREE_OK);
	}
	return (SYNTAX_ERROR);
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
	t_parse_tree	*assignment_node;

	if (*tok != NULL && is_redirection_token((*tok)->token->type))
	{
		*prefix_node = alloc_parse_tree();
		if (prefix_node == NULL)
			return (MEMORY_ERROR);
	}
	while (*tok != NULL && is_redirection_token((*tok)->token->type))
	{
		redirect_node = NULL;
		if (is_io_redirect(tok, &redirect_node) != SUBTREE_OK)
			return (free_parse_tree(*prefix_node), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
		link_node(prefix_node, redirect_node);
	}
	return (SUBTREE_OK);
}
