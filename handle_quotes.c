/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/04 22:02:41 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_result(char **result, char *updated_result, t_exec_vars *vars)
{
    char *new_result;

	new_result = ft_strjoin(*result, updated_result);
    if (!check_null(new_result, &vars->error))
        return;
    free(*result); // Free the old string
    *result = new_result; // Assign the new string
}

void handle_error_and_free(t_exec_vars *vars, t_handle_vars *l_vars)
{
    if (!vars->error)
	{
        free(vars->args[vars->i]);
        vars->args[vars->i] = NULL;
        handle_quotes_final_assign(&vars->args[vars->i], l_vars->result, vars);
        free_handle_vars(l_vars);
    }
	free_handle_vars(l_vars);
}

void	handle_quotes_glob_1(t_p_tree **node, t_exec_vars *vars,
		t_env **env)
{
	t_handle_vars	l_vars;

	init_handle_vars(&l_vars, vars);
	vars->error = 0;
	while (**l_vars.current != '\0')
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(l_vars.current, l_vars.result, vars,
				env);
		else
			handle_no_quotes(&l_vars, vars, env, node);
		if (vars->end)
		{
			vars->end = 0;
			return (free_handle_vars(&l_vars));
		}
		if (vars->error)
			break ;
	}
	*l_vars.current = l_vars.current_start;
	handle_error_and_free(vars, &l_vars);
}

void update_args(t_exec_vars *vars, t_handle_vars *l_vars)
{
    free(vars->args[vars->i]);
    vars->args[vars->i] = ft_strjoin(*l_vars->result, l_vars->token);
    if (!check_null(vars->args[vars->i], &vars->error))
        return;
}

void	handle_no_current(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
			sizeof(l_vars->buffer));
		if (g_last_exit_status)
		{
			vars->error = 1;
			return ;
		}
		update_result(l_vars->result, l_vars->buffer, vars);
		if (vars->error)
			return (free(*l_vars->result));
		if (strchr((*node)->data->lexeme, '$') != NULL
			&& strchr(l_vars->buffer, ' '))
			vars->i = split_variable(*l_vars->result, vars->i, vars);
		if (vars->error)
			return (free(*l_vars->result));
		else
		{
        	free(vars->args[vars->i]);
        	vars->args[vars->i] = ft_strdup(*l_vars->result);
			if (!check_null(vars->args[vars->i], &vars->error))
				return (free(*l_vars->result));
    	}
	}
	else
		update_args(vars, l_vars);
	if (vars->error)
		return (free(*l_vars->result));
	vars->end = 1;
}

void	handle_with_current_dollar(t_handle_vars *l_vars,
			t_exec_vars *vars, t_env **env, t_p_tree **node)
{
	handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
		sizeof(l_vars->buffer));
	if (g_last_exit_status)
	{
		vars->error = 1;
		return ;
	}
	update_result(l_vars->result, l_vars->buffer, vars);
	if (vars->error)
		return (free(*l_vars->result));
	if (ft_strchr((*node)->data->lexeme, '$') != NULL
		&& ft_strchr(l_vars->buffer, ' '))
	{
		vars->i = split_variable(*l_vars->result, vars->i, vars);
		if (vars->error)
			return (free(*l_vars->result));
		free(*l_vars->result);
		*l_vars->result = ft_strdup(vars->args[vars->i]);
	}
}

void	handle_with_current(t_handle_vars *l_vars,
		t_exec_vars *vars, t_env **env, t_p_tree **node)
{
	char	delimiter;
	char	*new_result;

	delimiter = **l_vars->current;
	**l_vars->current = '\0';
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_with_current_dollar(l_vars, vars, env, node);
		if (vars->error)
			return ;
	}
	else
	{
		update_result(l_vars->result, l_vars->token, vars);
		if (vars->error)
			return ;
		/* new_result = ft_strjoin(*l_vars->result, l_vars->token);
    	if (!check_null(new_result, &vars->error))
        	return ;
    	free(*l_vars->result); // Free the old string
    	*l_vars->result = new_result; // Assign the new string */
	}
	**l_vars->current = delimiter;
	if (**l_vars->current == '\'')
		vars->inside_single_quotes = 1;
	else if (**l_vars->current == '\"')
		vars->inside_double_quotes = 1;
	(*l_vars->current)++;
}

void	handle_no_quotes(t_handle_vars *l_vars, t_exec_vars *vars,
				t_env **env, t_p_tree **node)
{
	char *temp;

	l_vars->token = *l_vars->current;
	temp = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (temp == NULL)
	{
		handle_no_current(l_vars, vars, env, node);
		return ;
	}
	*l_vars->current = temp;//NEEDED??
	handle_with_current(l_vars, vars, env, node);
}
