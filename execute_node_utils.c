/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:08 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/05 14:24:21 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_exec_vars(t_exec_vars *vars)
{
	int	i;

	vars->fd_in = 0;
	vars->fd_out = 1;
	vars->capacity = 10;
	vars->args = malloc(vars->capacity * sizeof(char *));
	if (!vars->args)
	{
		vars->error = 1;
		return ;
	}
	i = 0;
	while (i < vars->capacity)
		vars->args[i++] = NULL;
	vars->i = 0;
	vars->error = 0;
	vars->inside_single_quotes = 0;
	vars->inside_double_quotes = 0;
	vars->end = 0;
}

void	expand_exec_vars(t_exec_vars *vars)
{
	int		new_capacity;
	char	**new_args;
	int		i;
	size_t	original_size;
	size_t	new_size;

	i = vars->capacity;
	new_capacity = vars->capacity * 2;
	original_size = vars->capacity * sizeof(char *);
	new_size = new_capacity * sizeof(char *);
	new_args = ft_realloc(vars->args, original_size, new_size);
	if (new_args == NULL)
	{
		vars->error = 1;
		return ;
	}
	vars->args = new_args;
	while (i < new_capacity)
		vars->args[i++] = NULL;
	vars->capacity = new_capacity;
}

int	split_variable(char *arg, int i, t_exec_vars *vars)
{
	char	**split_args;
	int		j;

	j = 0;
	split_args = ft_split(arg, ' ');
	if (!split_args)
	{
		vars->error = 1;
		return (i);
	}
	free(vars->args[vars->i]);
	while (split_args[j])
	{
		vars->args[i + j] = split_args[j];
		j++;
	}
	free(split_args);
	return (i + (j - 1));
}
