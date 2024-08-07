/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 17:21:40 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_space_tabs(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((unsigned char)str[i] != 0xFF)
			return (0);
		i++;
	}
	return (1);
}

static int	complex_handle_node_data(t_free_data *exec_data, t_exec_vars *vars)
{
	while (exec_data->tree != NULL)
	{
		if (exec_data->tree->data != NULL)
		{
			if (handle_node_data(&exec_data->tree, vars, exec_data,
				&exec_data->hd_fd))
				return (g_last_exit_status);
			if (vars->args[vars->i] != NULL)
			{
				free(vars->args[vars->i]);
				vars->args[vars->i] = NULL;
			}
		}
		if (vars->i < vars->capacity - 1)
		{
			vars->args[vars->i] = NULL;
			expand_exec_vars(vars);
			if (vars->error)
				return (g_last_exit_status);
		}
		exec_data->tree = exec_data->tree->child;
	}
	return (0);
}

int	handle_node_data(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data, int *here_docs)
{
	if (is_only_space_tabs((*node)->data->lexeme))
		return (0);
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
		return (handle_redirection(node, vars, exec_data, here_docs));
	vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
	if (!vars->args[vars->i])
	{
		vars->error = 1;
		return (print_err(1, 2, "my(s)hell: ft_strdup error\n"), 1);
	}
	handle_quotes_glob(node, vars, exec_data);
	if (vars->error)
	{
		g_last_exit_status = 1;
		return (1);
	}
	if (!*vars->args[vars->i] && ft_strchr((*node)->data->lexeme, '$') != NULL
		&& ft_strchr((*node)->data->lexeme, '"') == NULL
		&& ft_strchr((*node)->data->lexeme, '\'') == NULL)
	{
	}
	else
		vars->i++;
	return (0);
}

int	execute_node(t_free_data *exec_data)
{
	t_exec_vars	*vars;

	if (exec_data->tree == NULL)
		return (0);
	vars = malloc(sizeof(t_exec_vars));
	if (!vars)
	{
		return (print_err(1, 2,
				"my(s)hell: execute_node malloc error 10\n"), 1);
	}
	init_exec_vars(vars);
	if (vars->error)
		return (free(vars), g_last_exit_status);
	if (complex_handle_node_data(exec_data, vars))
		return (free_vars(vars), g_last_exit_status);
	if (vars->args[vars->i] != NULL)
		free(vars->args[vars->i]);
	vars->args[vars->i] = NULL;
	execute_command(vars, exec_data);
	if (exec_data->hd_fd != -1)
		close(exec_data->hd_fd);
	free_vars(vars);
	return (g_last_exit_status);
}
