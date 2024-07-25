/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:26:22 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_underscore(t_exec_vars *vars, t_free_data *exec_data)
{
	if (ft_strcmp(vars->args[0], "echo") == 0)
	{
		if (update_add_env_var(&exec_data->env, "_", vars->args[1])) //maybe different error handling
			return (g_last_exit_status);
	}
	else if (update_add_env_var(&exec_data->env, "_", vars->args[0])) //maybe different error handling
			return (g_last_exit_status);
	return (0);
	/* if (ft_strcmp(vars->args[0], "exit") == 0
		|| ft_strcmp(vars->args[0], "cd") == 0
		|| ft_strcmp(vars->args[0], "pwd") == 0
		|| ft_strcmp(vars->args[0], "export") == 0
		|| ft_strcmp(vars->args[0], "unset") == 0
		|| ft_strcmp(vars->args[0], "env") == 0
		|| (ft_strcmp(vars->args[0], "echo") == 0 && !vars->args[1]))
	{
		if (update_add_env_var(&exec_data->env, "_", vars->args[0])) //maybe different error handling
			return (g_last_exit_status);
	}
	else if (ft_strcmp(vars->args[0], "echo") == 0)
	{
		if (update_add_env_var(&exec_data->env, "_", vars->args[1])) //maybe different error handling
			return (g_last_exit_status);
	}
	return (0); */
}

int	exec_builtins(t_exec_vars *vars, t_free_data *exec_data)
{
	//if (update_underscore(vars, exec_data))
		//return (g_last_exit_status);
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, exec_data));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "pwd") == 0)
		return (exec_pwd());//(&exec_data->env));
	else if (ft_strcmp(vars->args[0], "echo") == 0)
		return (exec_echo(vars));
	else if (ft_strcmp(vars->args[0], "export") == 0)
		return (exec_export(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "unset") == 0)
		return (exec_unset(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "env") == 0)
		return (exec_env(vars->args, exec_data->environment));
	return (3);
}
