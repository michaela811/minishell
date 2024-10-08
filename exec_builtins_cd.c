/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_cd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/09 15:11:19 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_update_pwd(t_env **env)
{
	if (update_pwd(env))
	{
		g_last_exit_status = 1;
		return (1);
	}
	return (0);
}

int	change_directory_and_update(char *path, t_env **env, char **args)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
	{
		if (errno == ENOENT)
			print_err(1, 2, "my(s)hell: %s: %s: No such file or directory\n",
				args[0], path);
		else
			print_err(1, 2, "my(s)hell: %s: %s: Error accessing path\n",
				args[0], path);
		return (1);
	}
	if (!S_ISDIR(path_stat.st_mode))
		return (print_err(1, 2, "my(s)hell: %s: %s: Not a directory\n",
				args[0], path), 1);
	if (access(path, X_OK) == -1 && access(path, F_OK) == 0)
		return (print_err(1, 2, "my(s)hell: %s: %s: Permission denied\n",
				args[0], path), 1);
	if (chdir(path) != 0)
		return (print_err(1, 2,
				"my(s)hell: %s: %s: Unable to change directory\n", args[0],
				path), 1);
	if (ft_update_pwd(env))
		return (1);
	return (g_last_exit_status);
}

int	update_pwd(t_env **env)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = get_env_var(*env, "PWD");
	if (update_add_env_var(env, "OLDPWD", old_pwd))
		return (1);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (print_err(1, 2, "my(s)hell: cd: error retrieving current"
				" directory: getcwd: cannot access parent directories: "
				"No such file or directory\n"), 1);
	if (update_add_env_var(env, "PWD", pwd))
		return (free(pwd), 1);
	return (free(pwd), 0);
}

int	exec_cd(char **args, t_env **env)
{
	if (args[1] != NULL && args[2])
		return (print_err(1, 2, "my(s)hell: %s: too many arguments\n",
				args[0]), g_last_exit_status);
	else if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		return (change_directory_and_update(get_env_var(*env, "HOME"),
				env, args));
	else if (ft_strcmp(args[1], "..") == 0)
		return (change_directory_and_update("..", env, args));
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (get_env_var(*env, "OLDPWD") == NULL)
			return (print_err(1, 2, "my(s)hell: %s: OLDPWD not set\n",
					args[0]), g_last_exit_status);
		change_directory_and_update(get_env_var(*env, "OLDPWD"),
			env, args);
		ft_printf_fd(STDOUT_FILENO, "%s\n", get_env_var(*env, "PWD"));
		return (g_last_exit_status);
	}
	else
		return (change_directory_and_update(args[1], env, args));
}
