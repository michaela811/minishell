/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_exec_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:25 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 10:52:11 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_exec_vars *vars, t_free_data *exec_data)
{
	int	return_builtins;

	if (!vars->args[0])
		return (0);
	if (update_underscore(vars, exec_data))
		return (g_last_exit_status);
	exec_data->environment = env_list_to_array(exec_data->env);
	if (exec_data->environment == NULL)
	{
		g_last_exit_status = 1;
		return (g_last_exit_status);
	}
	return_builtins = exec_builtins(vars, exec_data);
	if (return_builtins == 3)
		handle_fork(vars, &exec_data->env, exec_data);
	return (g_last_exit_status);
}

int	handle_fork(t_exec_vars *vars, t_env **env, t_free_data *exec_data)
{
	pid_t	pid;
	int		status;

	pid = 0;
	pid = fork();
	if (pid == -1)
	{
		print_err(128, 2, "my(s)hell: fork\n");
		exit(g_last_exit_status);
	}
	else if (pid == 0)
	{
		handle_child_cmd(vars, env, exec_data->environment, exec_data);
		exit (g_last_exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	return (g_last_exit_status);
}
