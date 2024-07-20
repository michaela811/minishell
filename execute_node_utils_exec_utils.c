/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils_exec_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/17 18:57:03 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	handle_fork(t_exec_vars *vars, t_env **env, char **environment)
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
		handle_child_cmd(vars, env, environment);
		exit (g_last_exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	return (g_last_exit_status);
}

static int	print_and_exit(t_exec_vars *vars)
{
	print_err(127, 2, "my(s)hell: execve\n", vars->args[0]);
	exit(127);
}

void cleanup(t_exec_vars *vars)
{
    if (vars->fd_in != 0)
        close(vars->fd_in);
    if (vars->fd_out != 1)
        close(vars->fd_out);
}

int remove_empty_args(t_exec_vars *vars)
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
}

int	handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment)
{
	char	*path;

	path = NULL;
	if (vars->fd_in != 0)
	{
		dup2(vars->fd_in, 0);
		close(vars->fd_in);
	}
	if (vars->fd_out != 1)
	{
		dup2(vars->fd_out, 1);
		close(vars->fd_out);
	}
	//if (remove_empty_args(vars))
		//exit (g_last_exit_status);
	if (access(vars->args[0], F_OK | X_OK) == 0 && vars->args[0][0] == '/')
		path = vars->args[0];
	/* else
		get_path(vars->args[0], *env, &path); */
	if ((err_check_fork(vars, env, &path)) != 0)
	{
		cleanup(vars);
		exit (g_last_exit_status);
	}
	//if (execve(path, vars->args, environment) < 0)
	g_last_exit_status = execve(path, vars->args, environment);
	if (g_last_exit_status < 0)
	{
		cleanup(vars);
		print_and_exit(vars);
	}
	//g_last_exit_status = 0;
	exit (EXIT_SUCCESS);
}

int	path_status_2(t_exec_vars *vars, char **path)
{
	if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.'
			&& vars->args[0][1] == '/')
			return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
					vars->args[0]), 126);
		//if (access(vars->args[0], F_OK) == -1 && )
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				*path), 127);
}

int path_status_1(t_exec_vars *vars, t_env **env)
{
	if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.'
			&& vars->args[0][1] == '/')
			return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
					vars->args[0]), 126);
		if (access(vars->args[0], F_OK) == 0 && !find_env_var(*env, "PATH"))
			return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
					vars->args[0]), 126);
		//if (access(vars->args[0], F_OK) == -1 && )
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				vars->args[0]), 127);
}

int control_dot(t_exec_vars *vars)
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
	if (access(vars->args[0], F_OK | X_OK) == 0 && ft_strcmp(vars->args[0], ".") == 0)
		//return (control_dot(vars));
	{
		if (!vars->args[1])
			return (print_err(2, 2, "my(s)hell: %s: filename argument required\n",
					vars->args[0]), 2);
		else
			return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				vars->args[0]), 127);
	}
	if (access(vars->args[0], F_OK | X_OK) == 0 &&
	(vars->args[0][0] == '/' || vars->args[0][0] == '.' || vars->args[0][0] == ':'))
		return(*path = vars->args[0], 0);
	else
		path_status = get_path(vars->args[0], *env, path);
	if (path_status == 1)
		return (g_last_exit_status);
	if (path_status == -2)
		//return(path_status_2(vars, path));
	{
		if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.'
			&& vars->args[0][1] == '/')
			return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
					vars->args[0]), 126);
		//if (access(vars->args[0], F_OK) == -1 && )
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				*path), 127);
	}
	if (path_status == -1 || vars->args[0][0] == '\0')
		//|| vars->args[0][0] == '.')
		//return (path_status_1(vars, env));
	{
		if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.'
			&& vars->args[0][1] == '/')
			return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
					vars->args[0]), 126);
		if (access(vars->args[0], F_OK) == 0 && !find_env_var(*env, "PATH"))
			return (print_err(126, 2, "my(s)hell: %s: Permission denied\n",
					vars->args[0]), 126);
		//if (access(vars->args[0], F_OK) == -1 && )
		return (print_err(127, 2, "my(s)hell: %s: command not found\n",
				vars->args[0]), 127);
	}
	return ((g_last_exit_status = 0), EXIT_SUCCESS);
}

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
	return (0);
}
