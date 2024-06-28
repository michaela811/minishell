#include "minishell.h"

void	free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->lexeme == NULL)
		return ;
	free(tok->lexeme);
	tok->lexeme = NULL;
	free(tok);
	tok = NULL;
}

void	free_parse_tree(t_p_tree *tree)
{
	t_p_tree	*sibling;
	t_p_tree	*child;

	if (tree == NULL)
		return ;
	child = tree->child;
	sibling = tree->sibling;
	if (sibling != NULL)
		free_parse_tree(sibling);
	if (child != NULL)
		free_parse_tree(child);
	free(tree);
	tree = NULL;
}

void	free_token_list(t_token_list *list)
{
	t_token_list	*current;
	t_token_list	*next;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		if (current->token->lexeme != NULL)
		{
			free(current->token->lexeme);
			current->token->lexeme = NULL;
		}
		if (current->token != NULL)
		{
			free(current->token);
			current->token = NULL;
		}
		if (current != NULL)
		{
			free(current);
			current = NULL;
		}
		current = next;
	}
	list = NULL;
}

void	free_array(char **array)
{
	size_t	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	free_env(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}
