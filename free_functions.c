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
	if (tree->sibling != NULL) {
        free_parse_tree(tree->sibling);
        tree->sibling = NULL;
    }
    if (tree->child != NULL) {
        free_parse_tree(tree->child);
        tree->child = NULL;
    }
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
		//printf("%d: %s\n", i, env_array[i]);
		free(env_array[i]);
		i++;
	}
	free(env_array);
	env_array = NULL;
}

void	free_exit_data(t_free_data *exec_data)
{
    if (exec_data)
	{
		/*free_env_array(vars->args);
		vars->args = NULL;
		free(vars);*/
		if (exec_data->token_list_start) {
            free_token_list(exec_data->token_list_start);
			//free(exec_data->token_list_start);
			exec_data->token_list_start = NULL;
        }
        if (exec_data->tree_start) {
            free_parse_tree(exec_data->tree_start);
			exec_data->tree_start = NULL;
        }
        if (exec_data->env) {
            free_env(exec_data->env);
			exec_data->env = NULL;
        }
        if (exec_data->environment) {
			free_env_array(exec_data->environment);
			exec_data->environment = NULL;
        }
        free(exec_data);
		exec_data = NULL;
		clear_history();
    }
}

void	free_command_data(t_free_data *exec_data)
{
    if (exec_data) {
		if (exec_data->token_list_start) {
            free_token_list(exec_data->token_list_start);
			exec_data->token_list_start = NULL;
        }
        if (exec_data->tree_start) {
            free_parse_tree(exec_data->tree_start);
			exec_data->tree_start = NULL;
        }
        //if (exec_data->env) {
            //free_env(exec_data->env);
        //}
        if (exec_data->environment) {
			free_env_array(exec_data->environment);
			exec_data->environment = NULL;
        }
        //free(exec_data);
    }
}
