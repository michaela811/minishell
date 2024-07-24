/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 16:30:45 by mmasarov         ###   ########.fr       */
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

static int	complex_handle_node_data(t_free_data *exec_data, t_exec_vars *vars, t_hd_data *here_docs)
{
	while (exec_data->tree != NULL)
	{
		if (exec_data->tree->data != NULL)
		{
			handle_node_data(&exec_data->tree, vars, &exec_data->env, here_docs);
			if (vars->args[0] == NULL)
				vars->i = 0;
			if (vars->error != 0)
				return (g_last_exit_status);
		}
		check_capacity(vars);
		exec_data->tree = exec_data->tree->child;
	}
	return (0);
}

void	handle_node_data(t_p_tree **node, t_exec_vars *vars, t_env **env,
		t_hd_data *here_docs)
{
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
		return (handle_redirection(node, vars, env, here_docs));
	vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
	if (!vars->args[vars->i])
	{
		vars->error = 1;
		return (print_err(1, 2, "my(s)hell: ft_strdup error\n"));
	}
	handle_quotes_glob(node, vars, env);
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

int	execute_node(t_free_data *exec_data, t_hd_data *here_docs)
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
	if (complex_handle_node_data(exec_data, vars, here_docs))
		return (g_last_exit_status);
	vars->args[vars->i] = NULL;
	execute_command(vars, exec_data);
	free_env_array(vars->args);
	if (vars->open_fd_in)
		close(vars->fd_in);
	free(vars);
	return (g_last_exit_status);
}
