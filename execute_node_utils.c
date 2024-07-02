/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:08 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:36:10 by mmasarov         ###   ########.fr       */
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

	i = vars->capacity;
	new_capacity = vars->capacity * 2;
	new_args = realloc(vars->args, new_capacity * sizeof(char *));
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

void	handle_node_data(t_p_tree **node, t_exec_vars *vars, t_env **env)
{
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
		return (handle_redirection(node, vars, env));
	vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
	if (!vars->args[vars->i])
	{
		vars->error = 1;
		return (print_err(1, 2, "my(s)hell: ft_strdup error\n"));
	}
	handle_quotes_glob_1(node, vars, env);
	if (vars->error)
	{
		g_last_exit_status = 1;
		return ;
	}
	vars->i++;
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
	free(vars->args[vars->i]);//check later if needed!!!
	while (split_args[j])
	{
		vars->args[i + j] = split_args[j];
		j++;
	}
	free(split_args);
	return (i + (j - 1));
}

void	handle_redirection(t_p_tree **node, t_exec_vars *vars, t_env **env)
{
	if ((*node)->data->type == RED_FROM)
		return (handle_redirection_from(node, vars, env));
	else if ((*node)->data->type == RED_TO)
		return (handle_redirection_to(node, vars, env));
	else if ((*node)->data->type == APPEND)
		return (handle_redirection_append(node, vars, env));
	else if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars));
}
