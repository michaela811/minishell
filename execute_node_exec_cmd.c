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

int	execute_command(t_exec_vars *vars, t_free_data *exec_data, t_hd_data *here_docs)
{
	int	return_builtins;

	if (!vars->args[0])
		return (0);
	exec_data->environment = env_list_to_array(exec_data->env);
	if (exec_data->environment == NULL)
	{
		g_last_exit_status = 1;
		return (free_command_data(exec_data), g_last_exit_status); //added
	}
	return_builtins = exec_builtins(vars, exec_data);
	if (return_builtins == 3)
		handle_fork(vars, &exec_data->env, exec_data, here_docs);
	return(g_last_exit_status);
}

int	handle_fork(t_exec_vars *vars, t_env **env, t_free_data *exec_data, t_hd_data *here_docs)
{
	pid_t	pid;
	int		status;

	pid = 0;
	if (update_add_env_var(env, "_", vars->args[0]))//maybe different error handling
			return (g_last_exit_status);
	pid = fork();
	if (pid == -1)
	{
		print_err(128, 2, "my(s)hell: fork\n");
		exit(g_last_exit_status);
	}
	else if (pid == 0)
	{
		handle_child_cmd(vars, env, exec_data->environment, exec_data);
		if (here_docs != NULL) //not necessary, isnt execute anyway
		{
			close(here_docs->fd);
			free(here_docs);
		}
		exit (g_last_exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	return (g_last_exit_status);
}

/* int	get_args_count(char **args)
{
	int	count;

	count = 0;
	while (args[count] != NULL)
		count++;
	return (count);
} */
