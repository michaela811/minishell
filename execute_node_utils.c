#include "minishell.h"

void	init_exec_vars(t_exec_vars *vars)
{
	int	i;

	vars->fd_in = 0;
	vars->fd_out = 1;
	vars->args = malloc(10 * sizeof(char *));  // Allocate memory for args
    if (!vars->args)
    {
        // Handle memory allocation error
        return;
    }
	i = 0;
	while (i < 10)
		vars->args[i++] = NULL;
	vars->i = 0;
	vars->error = 0;
}

void	handle_node_data(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
	{
		handle_redirection(node, vars);
		return ;
	}
	if ((*node)->data->lexeme[0] == '$' && strcmp((*node)->data->lexeme, "$?") == 0)
	{
		vars->args[vars->i] = ft_strdup(ft_itoa(g_last_exit_status));
		if (!vars->args[vars->i])
		{
			vars->error = 1;
			return(printf_global_error(1, 2, "my(s)hell: ft_strdup error\n"));
		}
	}
	//else if ((*node)->data->lexeme[0] == '"' || (*node)->data->lexeme[0] == 39 || (*node)->data->lexeme[0] == '$')
	else if ((*node)->data->lexeme)
	{
		handle_quotes_global(node, vars->args, vars->i, env);
		if (g_last_exit_status)
		{
			vars->error = 1;
			return ;
		}
		if (ft_strcmp(vars->args[vars->i], "") == 0)
			return ;
	}
	else
	{
		vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
		if (!vars->args[vars->i])
		{
			vars->error = 1;
			return(printf_global_error(1, 2, "my(s)hell: ft_strdup error\n"));
		}
	}
	if ((*node)->data->lexeme[0] == '$' && ft_strchr(vars->args[vars->i], ' '))
		vars->i = split_variable(vars->args[vars->i], vars->i, vars);//ADD ERROR HANDLING
	vars->i++;
}

int split_variable(char *arg, int i, t_exec_vars *vars)
{
    char **split_args;
    int j;

    j = 0;
    split_args = ft_split(arg, ' ');
    if (!split_args)
        return i;
    while (split_args[j])
	{
        vars->args[i + j] = split_args[j];
        j++;
    }
    free(split_args);
    return i + (j - 1);
}

void	handle_redirection(t_parse_tree **node, t_exec_vars *vars)
{
	if ((*node)->data->type == RED_FROM)
		return (handle_redirection_from(node, vars));
	else if ((*node)->data->type == RED_TO)
		return (handle_redirection_to(node, vars));
	else if ((*node)->data->type == APPEND)
		return (handle_redirection_append(node, vars));
	else if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars));
}
