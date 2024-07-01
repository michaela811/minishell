/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_parse_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:36:16 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_parse_tree(t_free_data *exec_data)
{
	if (exec_data->tree == NULL)
		return (0);
	if (exec_data->tree->sibling)
	{
		is_there_here_doc(exec_data);
		execute_pipeline(exec_data);
	}
	else
		execute_node(exec_data);
	return (g_last_exit_status);
}

void	print_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		printf("args[%d] = %s\n", i, args[i]);
		i++;
	}
}

static void	check_capacity(t_exec_vars *vars)
{
	if (vars->i > vars->capacity - 1)
	{
		expand_exec_vars(vars);
		if (vars->error)
			free_env_array(vars->args);
	}
}

static int	complex_handle_node_data(t_free_data *exec_data, t_exec_vars *vars)
{
	while (exec_data->tree != NULL)
	{
		if (exec_data->tree->data != NULL)
		{
			handle_node_data(&exec_data->tree, vars, &exec_data->env);
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
		return (g_last_exit_status);
	vars->args[vars->i] = NULL;
	execute_command(vars, exec_data);
	free_env_array(vars->args);
	free(vars);
	return (g_last_exit_status);
}
