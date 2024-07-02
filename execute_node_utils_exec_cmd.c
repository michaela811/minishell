/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils_exec_cmd.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:25 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:35:33 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_exec_vars *vars, t_free_data *exec_data)
{
	int	return_builtins;

	if (!vars->args[0])
		return (0);
	exec_data->environment = env_list_to_array(exec_data->env);
	if (exec_data->environment == NULL)
	{
		g_last_exit_status = 1;
		return (free_command_data(exec_data), g_last_exit_status);
	}
	return_builtins = exec_builtins(vars, exec_data);
	if (return_builtins == 3)
		handle_fork(vars, &exec_data->env, exec_data->environment);
	return (g_last_exit_status);
}

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (-1);
	return (S_ISDIR(path_stat.st_mode));
}

int	get_args_count(char **args)
{
	int	count;

	count = 0;
	while (args[count] != NULL)
		count++;
	return (count);
}
