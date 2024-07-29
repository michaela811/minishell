/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_child_cmd.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:35:48 by mmasarov         ###   ########.fr       */
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
	if (vars->fd_in != 0)
		close(vars->fd_in);
	if (vars->fd_out != 1)
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

int	handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment, t_free_data *exec_data)
{
	char	*path;

	path = NULL;
	set_fds(vars->fd_in, vars->fd_out);
	if (access(vars->args[0], F_OK | X_OK) == 0 && vars->args[0][0] == '/')
		path = vars->args[0];
	if ((err_check_fork(vars, env, &path)) != 0)
	{
		cleanup(vars);
		free_env_array(vars->args);
		if (vars->open_fd_in)
			close(vars->fd_in);
		free(vars);
		if (exec_data->token_list_start)
		{
			free_token_list(exec_data->token_list_start);
			exec_data->token_list_start = NULL;
		}
		if (exec_data->tree_start)
		{
			free_parse_tree(exec_data->tree_start);
			exec_data->tree_start = NULL;
		}
		if (exec_data->env)
		{
			free_env(exec_data->env);
			exec_data->env = NULL;
		}
		if (exec_data->environment)
		{
			free_env_array(exec_data->environment);
			exec_data->environment = NULL;
		}
		//if (exec_data)
		//	free(exec_data);
		exit (g_last_exit_status);
	}
	g_last_exit_status = execve(path, vars->args, environment);
	if (g_last_exit_status < 0)
	{
		cleanup(vars);
		print_and_exit(vars);
	}
	exit (EXIT_SUCCESS);
}

/* int remove_empty_args(t_exec_vars *vars)
{
	int i;

	while (vars->args[0])
	{
		if (ft_strcmp(vars->args[0], "") == 0)
		{
			free(vars->args[0]);
			if (vars->args[1] != NULL)
			{
				i = 0;
				while (vars->args[i + 1] != NULL)
				{
					vars->args[i] = vars->args[i + 1];
					i++;
				}
				vars->args[i] = NULL;
				vars->i = i;
			}
			else
			{
				g_last_exit_status = 0;
				return (1);
			}
		}
		else
			break;
	}
	return (0);
} */
