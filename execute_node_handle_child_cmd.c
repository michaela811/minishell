/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_child_cmd.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 16:23:43 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	print_and_exit(t_exec_vars *vars)
{
	print_err(127, 2, "my(s)hell: execve\n", vars->args[0]);
	exit(127);
}

void	cleanup(t_exec_vars *vars)
{
	if (vars->fd_in)
		close(vars->fd_in);
	if (vars->fd_out)
		close(vars->fd_out);
}

void	set_fds(int fd_in, int fd_out)
{
	if (fd_in != 0)
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (fd_out != 1)
	{
		dup2(fd_out, 1);
		close(fd_out);
	}
}

void	clean_child_cmd(t_exec_vars *vars, t_free_data *exec_data)
{
	cleanup(vars);
	free_env_array(vars->args);
	free(vars);
	vars = NULL;
	free_exit_data(exec_data);
	if (exec_data->hd_fd != -1)
		close(exec_data->hd_fd);
	close(STDIN_FILENO);
	if (STDOUT_FILENO)
		close(STDOUT_FILENO);
}

int	handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment,
	t_free_data *exec_data)
{
	char	*path;

	path = NULL;
	set_fds(vars->fd_in, vars->fd_out);
	if (access(vars->args[0], F_OK | X_OK) == 0 && vars->args[0][0] == '/')
		path = vars->args[0];
	if ((err_check_fork(vars, env, &path)) != 0)
	{
		clean_child_cmd(vars, exec_data);
		exit (g_last_exit_status);
	}
	g_last_exit_status = execve(path, vars->args, environment);
	if (g_last_exit_status < 0)
	{
		clean_child_cmd(vars, exec_data);
		print_and_exit(vars);
	}
	exit (EXIT_SUCCESS);
}
