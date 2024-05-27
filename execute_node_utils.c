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

/* void	handle_quotes_glob(char **arg, t_env **env)
{
    char *result;
    int k;
    int j;
    char quote;
    char *start;
    char	buffer[1024] = "";

    result = malloc(ft_strlen(*arg) + 1);
    if (!result)
        return (printf_global_error(1, 2, "echo: memory allocation\n"));
    k = 0;
    j = 0;
    while ((*arg)[k] != '\0')
    {
        if ((*arg)[k] == '\'' || (*arg)[k] == '\"')
        {
            quote = (*arg)[k++];
            start = &(*arg)[k];
            while ((*arg)[k] != '\0' && (*arg)[k] != quote)
                result[j++] = (*arg)[k++];
            if ((*arg)[k] != quote)
            {
                g_last_exit_status = 1;
                return(free(result));
            }
            //result[j] = '\0';
            if (quote == '\"' && *start != '\"')
            {
                handle_dollar_sign(&start, buffer, env);
                free(result);
                result = malloc(ft_strlen(buffer) + 1);
                if (!result)
                return (printf_global_error(1, 2, "echo: memory allocation\n"));
                ft_strcpy(result, buffer);
                j = ft_strlen(result);
            }
            k++;
        }
        else if ((*arg)[k] == '$')
        {
            start = &(*arg)[k];
            handle_dollar_sign(&start, buffer, env);
            free(result);
            result = malloc(ft_strlen(*arg) + ft_strlen(buffer) + 1);
            if (!result)
                return (printf_global_error(1, 2, "echo: memory allocation\n"));
            ft_strcpy(result, buffer);
            j = ft_strlen(result);
            k = start - *arg;
        }
        else
            result[j++] = (*arg)[k++];
    }
    result[j] = '\0';
    free(*arg);
    *arg = result;
}

void	handle_node_data(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
    int index;
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
	    return (handle_redirection(node, vars));
    vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
	if (!vars->args[vars->i])
	{
		vars->error = 1;
		return(printf_global_error(1, 2, "my(s)hell: ft_strdup error\n"));
	}
    index = vars->i;
    if ((*node)->data->lexeme[0] == '$' && ft_strchr(vars->args[vars->i], ' '))
        vars->i = split_variable(vars->args[vars->i], vars->i, vars);//ADD ERROR HANDLING
    while (index <= vars->i)
    {
        handle_quotes_glob(&vars->args[index++], env);
        if (g_last_exit_status)
        {
            vars->error = 1;
            return ;
        }
    }
    vars->i++;
} */

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
	else if ((*node)->data->lexeme)
	{
		//hanlde_quotes_glob(node, vars->args, vars->i, env);
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
