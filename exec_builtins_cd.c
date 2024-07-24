/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:33:43 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_directory_and_update(char *path, t_env **env, char **args)
{
	if (access(path, X_OK) == -1 && access(path, F_OK) == 0)
			return (print_err(1, 2, "my(s)hell: %s: %s: Permission denied\n", args[0],
					path), 1);
	if (chdir(path) != 0)
	{
		print_err(1, 2,
			"my(s)hell: %s: %s: No such file or directory\n", args[0], path);
		return (1);
	}
	if (update_pwd(env))
	{
		g_last_exit_status = 1;
		return (1);
	}
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

int	update_pwd(t_env **env)
{
	char	*pwd;
	char	*cwd;

	pwd = get_env_var(*env, "PWD");//maybe add if PWD doesn't exist?
	if (!pwd)
		pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (print_err(1, 2, "my(s)hell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory"), 1);
	if (update_add_env_var(env, "OLDPWD", pwd))
		return (1);
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (print_err(1, 2, "my(s)hell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory"), 1);
		//return (perror("getcwd"), 1);
	if (update_add_env_var(env, "PWD", cwd))
		return (free(cwd), 1);
	return (free(cwd), 0);
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
		printf("%s\n", get_env_var(*env, "PWD"));
		return (g_last_exit_status);
	}
	else
		return (change_directory_and_update(args[1], env, args));
}