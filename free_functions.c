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

/* void	free_parse_tree(t_parse_tree *tree)
{
	t_parse_tree	*sibling;
	t_parse_tree	*child;

	if (tree == NULL)
		return ;
	child = tree->child;
	sibling = tree->sibling;
	if (sibling != NULL)
		free_parse_tree(sibling);
	if (child != NULL)
		free_parse_tree(child);
	if (tree->data != NULL)
	{
		free_token(tree->data);
		tree->data = NULL;
	}
	if (tree != NULL)
	{
		free(tree);
		tree = NULL;
	}
} */

void	free_parse_tree(t_parse_tree *tree)
{
    t_parse_tree	*sibling;
    t_parse_tree	*child;

    if (tree == NULL)
        return ;

    child = tree->child;
    sibling = tree->sibling;

    if (tree->data != NULL)
    {
        free_token(tree->data);
        tree->data = NULL;
    }

    free(tree);
    tree = NULL;

    if (sibling != NULL)
        free_parse_tree(sibling);
    if (child != NULL)
        free_parse_tree(child);
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

//We probably have this one already
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

void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i] != NULL)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	free_command_data(t_free_data *free_data)
{
    if (free_data)
	{
        if (free_data->tree)
            free_parse_tree(free_data->tree);
        if (free_data->token_list)
            free_token_list(free_data->token_list);
        if (free_data->environment)
			free_env_array(free_data->environment);
    }
}


void	free_data_exit(t_free_data *free_data)
{
    if (free_data) {
        if (free_data->tree) {
            free_parse_tree(free_data->tree);
        }
        if (free_data->env) {
            free_env(free_data->env);
        }
        if (free_data->token_list) {
            free_token_list(free_data->token_list);
        }
        if (free_data->environment) {
			free_env_array(free_data->environment);
        }
        free(free_data);
    }
}