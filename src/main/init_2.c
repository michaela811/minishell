/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 12:03:25 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/10 14:04:40 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	vars_set_null(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->buffer = NULL;
	l_vars->token = NULL;
	l_vars->delimiters = NULL;
	l_vars->current = NULL;
	l_vars->result = NULL;
	l_vars->current_start = NULL;
	if (init_handle_vars(l_vars, vars))
	{
		free_handle_vars(l_vars);
		vars->error = 1;
	}
	return ;
}

void	vars_set_null_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_p_tree **node)
{
	l_vars->buffer = NULL;
	l_vars->token = NULL;
	l_vars->delimiters = NULL;
	l_vars->current = NULL;
	l_vars->result = NULL;
	l_vars->current_start = NULL;
	if (init_handle_quote_redirect(l_vars, node))
	{
		free_handle_vars(l_vars);
		vars->error = 1;
	}
	return ;
}

int	initialize_vars_pipe_hd(char **rl_input, char **contents,
		t_handle_vars *l_vars)
{
	*rl_input = NULL;
	*contents = NULL;
	l_vars->buffer_size = 4096;
	l_vars->buffer = malloc(l_vars->buffer_size);
	if (l_vars->buffer == NULL)
	{
		print_err(1, 2, "my(s)hell: malloc error x1");
		free(l_vars->buffer);
		return (1);
	}
	return (0);
}
