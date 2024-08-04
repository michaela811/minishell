/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_child_cmd.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/04 15:08:39 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	print_and_exit(t_exec_vars *vars)
{
	print_err(127, 2, "my(s)hell: execve\n", vars->args[0]);
	exit(127);
}

void	cleanup(void)//(t_exec_vars *vars)
{
	/* if (vars->fd_in != 0)
		close(vars->fd_in);
	if (vars->fd_out != 1)
		close(vars->fd_out); */
	close(10);
	close(11);
}

void	set_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, 10);//if assign to 0 won't it be stdin? same for 1
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, 11);
		close(fd_out);
	}
}

void	clean_child_cmd(t_exec_vars *vars, t_free_data *exec_data)
{
	cleanup();
	free_env_array(vars->args);
	if (vars->open_fd_in)
		close(vars->fd_in);
	free(vars);
	vars = NULL;
	free_exit_data(exec_data);
	if (exec_data->hd_fd != -1)
		close(exec_data->hd_fd);
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
		/* cleanup(vars);
		free_env_array(vars->args);
		if (vars->open_fd_in)
			close(vars->fd_in);
		free(vars);
		vars = NULL;
		free_exit_data(exec_data);
		if (exec_data->hd_fd != -1)
			close(exec_data->hd_fd); */
		//close(STDOUT_FILENO);//NOT SURE!!!
		//close(STDIN_FILENO);//NOT SURE!!!
		exit (g_last_exit_status);
	}
	g_last_exit_status = execve(path, vars->args, environment);
	if (g_last_exit_status < 0)
	{
		clean_child_cmd(vars, exec_data);
		/* cleanup(vars);
		free_env_array(vars->args);
		if (vars->open_fd_in)
			close(vars->fd_in);
		free(vars);
		free_exit_data(exec_data);
		if (exec_data->hd_fd != -1)
			close(exec_data->hd_fd);
		if (path)
			free(path); */
		print_and_exit(vars);
	}
	exit (EXIT_SUCCESS);
}
