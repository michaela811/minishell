/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_error_messages.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:30:37 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 16:22:59 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	directory_check(char *arg_0)
{
	int	dir_check;

	dir_check = is_directory(arg_0);
	if (dir_check == -1)
		return (print_err(127, 2,
				"my(s)hell: %s: No such file or directory\n",
				arg_0), 127);
	else if (dir_check)
		return (print_err(126, 2,
				"my(s)hell: %s: Is a directory\n", arg_0), 126);
	if (access(arg_0, F_OK | X_OK) == -1)
		return (print_err(126, 2,
				"my(s)hell: %s: Permission denied\n", arg_0), 126);
	return (0);
}

int	path_status_2(t_exec_vars *vars, char **path)
{
	if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.'
			&& vars->args[0][1] == '/')
		return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
				vars->args[0]), 126);
	return (print_err(127, 2, "my(s)hell: %s: command not found\n",
			*path), free(*path), 127);
}

int	path_status_1(t_exec_vars *vars, t_env **env, char **path)
{
	if (*path == vars->args[0] && ft_strcmp(vars->args[0], "") != 0)
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				vars->args[0]), 127);
	if (*path != vars->args[0] && ft_strcmp(vars->args[0], "") == 0)
		return (free(*path), print_err(127, 2,
				"my(s)hell: %s: command not found\n", vars->args[0]), 127);
	if (*path != vars->args[0])
	{
		free(*path);
		*path = NULL;
	}
	if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.'
			&& vars->args[0][1] == '/')
		return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
				vars->args[0]), 126);
	if (access(vars->args[0], F_OK) == 0 && !find_env_var(*env, "PATH"))
		return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
				vars->args[0]), 126);
	return (print_err(127, 2, "my(s)hell: %s: No such file or directory\n",
			vars->args[0]), 127);
}

int	control_dot(t_exec_vars *vars)
{
	if (!vars->args[1])
		return (print_err(2, 2, "my(s)hell: %s: filename argument required\n",
				vars->args[0]), 2);
	else
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				vars->args[0]), 127);
}

int	err_check_fork(t_exec_vars *vars, t_env **env, char **path)
{
	int		path_status;

	if (ft_strchr(vars->args[0], '/'))
		if (directory_check(vars->args[0]))
			return (g_last_exit_status);
	if (access(vars->args[0], F_OK | X_OK) == 0
		&& ft_strcmp(vars->args[0], ".") == 0)
		return (control_dot(vars));
	if (access(vars->args[0], F_OK | X_OK) == 0
		&& ft_strcmp(vars->args[0], "..") == 0)
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				vars->args[0]), 127);
	if (access(vars->args[0], F_OK | X_OK) == 0
		&& (vars->args[0][0] == '/' || vars->args[0][0] == '.'
		|| vars->args[0][0] == ':'))
		return (*path = vars->args[0], 0);
	else
		path_status = get_path(vars->args[0], *env, path);
	if (path_status == 1 || path_status == 126 || path_status == 127)
		return (g_last_exit_status);
	if (path_status == -2)
		return (path_status_2(vars, path));
	if (path_status == -1 || vars->args[0][0] == '\0')
		return (path_status_1(vars, env, path));
	return ((g_last_exit_status = 0), EXIT_SUCCESS);
}
