/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 11:56:08 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int dollar_to_result(t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data)
{
	ft_memset(l_vars->buffer, '\0', sizeof(l_vars->buffer));
	if (handle_dollar_error(&l_vars->token, l_vars->buffer, vars, exec_data))
		return (1);
	if (update_result(l_vars->result, l_vars->buffer, vars))
		return (1);
	return (0);
}

void	handle_no_current(t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data, t_p_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		if (dollar_to_result(l_vars, vars, exec_data))
			return ;
		if (ft_strchr((*node)->data->lexeme, '$') != NULL
			&& ft_strchr(l_vars->buffer, ' '))
		{
			vars->i = split_variable(*l_vars->result, vars->i, vars);
			if (vars->error)
				return ;
		}
		else
		{
			free(vars->args[vars->i]);
			vars->args[vars->i] = ft_strdup(*l_vars->result);
			if (!check_null(vars->args[vars->i], &vars->error))
				return ;
		}
	}
	else
		update_args(vars, l_vars);
	if (vars->error)
		return ;
	vars->end = 1;
}

void	handle_with_current_dollar(t_handle_vars *l_vars,
			t_exec_vars *vars, t_free_data *exec_data, t_p_tree **node)
{
	if (handle_dollar_error(&l_vars->token, l_vars->buffer, vars, exec_data))
		return ;
	update_result(l_vars->result, l_vars->buffer, vars);
	if (vars->error)
		return ;
	if (ft_strchr((*node)->data->lexeme, '$') != NULL
		&& ft_strchr(l_vars->buffer, ' '))
	{
		vars->i = split_variable(*l_vars->result, vars->i, vars);
		if (vars->error)
			return (free(*l_vars->result));//still need it?
		free(*l_vars->result);
		if (buffer_end_space(l_vars->buffer))
		{
			*l_vars->result = ft_strjoin(vars->args[vars->i], " ");
			if (!check_null(*l_vars->result, &vars->error))
				return ;
		}
		else
		{
			*l_vars->result = ft_strdup(vars->args[vars->i]);
			if (!check_null(*l_vars->result, &vars->error))
				return ;
		}
	}
}

void	handle_with_current(t_handle_vars *l_vars,
		t_exec_vars *vars, t_free_data *exec_data, t_p_tree **node)
{
	char	delimiter;

	delimiter = **l_vars->current;
	**l_vars->current = '\0';
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_with_current_dollar(l_vars, vars, exec_data, node);
		if (vars->error)
			return ;
	}
	else
	{
		if (update_result(l_vars->result, l_vars->token, vars))
			return ;
	}
	**l_vars->current = delimiter;
	if (**l_vars->current == '\'')
		vars->inside_single_quotes = 1;
	else if (**l_vars->current == '\"')
		vars->inside_double_quotes = 1;
	(*l_vars->current)++;
}

void	handle_error_and_free(t_exec_vars *vars, t_handle_vars *l_vars)
{
	if (!vars->error)
	{
		free(vars->args[vars->i]);
		vars->args[vars->i] = NULL;
		handle_quotes_final_assign(&vars->args[vars->i], l_vars->result, vars);
		free_handle_vars(l_vars);
		return ;
	}
	free_handle_vars(l_vars);
}

void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars)
{
	*str1 = ft_strdup(*str2);
	check_null(*str1, &vars->error);
}
