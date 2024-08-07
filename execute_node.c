/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 15:23:30 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_capacity(t_exec_vars *vars)
{
	if (vars->i > vars->capacity - 1)
	{
		expand_exec_vars(vars);
		if (vars->error)
			free_env_array(vars->args);
	}
}

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
			handle_node_data(&exec_data->tree, vars, exec_data,
				&exec_data->hd_fd);
			if (vars->args[vars->i] != NULL)
			{
				free(vars->args[vars->i]);
				vars->args[vars->i] = NULL;
			}
			//if (vars->args == NULL)
			//	vars->i = 0;
			if (vars->error != 0)
				return (g_last_exit_status);
		}
		check_capacity(vars);
		exec_data->tree = exec_data->tree->child;
	}
	return (0);
}

void	handle_node_data(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data, int *here_docs)
{
	if (is_only_space_tabs((*node)->data->lexeme))
		return ;
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
		return (handle_redirection(node, vars, exec_data, here_docs));
	vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
	if (!vars->args[vars->i])
	{
		vars->error = 1;
		return (print_err(1, 2, "my(s)hell: ft_strdup error\n"));
	}
	handle_quotes_glob(node, vars, exec_data);
	if (vars->error)
	{
		g_last_exit_status = 1;
		return ;
	}
	if (!*vars->args[vars->i] && ft_strchr((*node)->data->lexeme, '$') != NULL
		&& ft_strchr((*node)->data->lexeme, '"') == NULL
		&& ft_strchr((*node)->data->lexeme, '\'') == NULL)
	{
	}
	else
		vars->i++;
}

int	execute_node(t_free_data *exec_data)
{
	t_exec_vars	*vars;

	vars = malloc(sizeof(t_exec_vars));
	if (!vars)
	{
		return (print_err(1, 2,
				"my(s)hell: execute_node malloc error\n"), 1);
	}
	init_exec_vars(vars);
	if (exec_data->tree == NULL)
		return (0);
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
