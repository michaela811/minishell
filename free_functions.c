#include "minishell.h"

void	free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->lexeme == NULL)
		return ;
	MY_FREE(tok->lexeme);
	tok->lexeme = NULL;
	MY_FREE(tok);
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
		MY_FREE(tree);
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

    /* if (tree->data != NULL)
    {
        free_token(tree->data);
        tree->data = NULL;
    } */
	if (sibling != NULL)
        free_parse_tree(sibling);
    if (child != NULL)
        free_parse_tree(child);
    MY_FREE(tree);
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
			MY_FREE(current->token->lexeme);
			current->token->lexeme = NULL;
		}
		if (current->token != NULL)
		{
			MY_FREE(current->token);
			current->token = NULL;
		}
		if (current != NULL)
		{
			MY_FREE(current);
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
		MY_FREE(array[i]);
	MY_FREE(array);
}

void	free_env(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		MY_FREE(current->name);
		MY_FREE(current->value);
		MY_FREE(current);
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
		MY_FREE(env_array[i]);
		i++;
	}
	MY_FREE(env_array);
	env_array = NULL;
}

void	free_exit_data(t_free_data *exec_data, t_exec_vars *vars)
{
    if (exec_data) {
		free_env_array(vars->args);
		vars->args = NULL;
		if (exec_data->token_list_start) {
            free_token_list(exec_data->token_list_start);
			//MY_FREE(exec_data->token_list_start);
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
        MY_FREE(exec_data);
		exec_data = NULL;
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
            //MY_FREE_env(exec_data->env);
        //}
        if (exec_data->environment) {
			free_env_array(exec_data->environment);
        }
        //MY_FREE(exec_data);
    }
}