/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 21:02:04 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_free_data	*init_command_data(char **envp)
{
	t_free_data	*exec_data;

	exec_data = malloc(sizeof(t_free_data));
	if (!exec_data)
		return (NULL);
	exec_data->env = init_environment(envp);
	/* if (exec_data->env == NULL)
	{
		print_err(1, 2, "init_environment failed\n");
		//free(exec_data);
		//return (NULL);
	} */
	exec_data->token_list = NULL;
	exec_data->environment = NULL;
	exec_data->tree = NULL;
	exec_data->token_list_start = NULL;
	exec_data->tree_start = NULL;
	return (exec_data);
}

/* char **alloc_and_init_str(int *error)
{
	char **result;

	result = malloc(sizeof(char *));
    if (!result)
	{
        *error = 1;
        return (NULL);
    }
    *result = ft_strdup("");
    if (!*result)
	{
        *error = 1;
        //free(result);
        return (NULL);
    }
    return (result);
} */

int	init_handle_vars(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->result = malloc(sizeof(char *));
	if (!l_vars->result)
		return (1);
	*l_vars->result = ft_strdup("");
	if (!*l_vars->result)
		return (1);
	l_vars->current = malloc(sizeof(char *));
	if (!l_vars->current)
		return (1);
	*l_vars->current = ft_strdup(vars->args[vars->i]);
	if (!*l_vars->current)
		return (1);
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	return (0);
}

int	init_handle_quote_redirect(t_handle_vars *l_vars, t_p_tree **node)
{
	l_vars->result = malloc(sizeof(char *));
	if (!l_vars->result)
		return (1);
	*l_vars->result = ft_strdup("");
	if (!*l_vars->result)
		return (free(l_vars->result), 1);
	l_vars->current = malloc(sizeof(char *));
	if (!l_vars->current)
		return (free(*l_vars->result) ,free(l_vars->result), 1);
	*l_vars->current = ft_strdup((*node)->child->data->lexeme);
	if (!*l_vars->current)
		return (free(l_vars->current), free(*l_vars->result), free(l_vars->result), 1);
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	return (0);
}

