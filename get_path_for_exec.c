/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_for_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 15:18:47 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	handle_initial_path(char **pre_path, t_env *env)
{
	if (handle_colon(pre_path, env))
		return (print_err(1, 2,
				"my(s)hell: malloc error in handle_colon function\n"), 1);
	return (0);
}

int	is_a_directory(char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISDIR(path_stat.st_mode));
}

int	exec_from_cwd(char *cmd, char **path)
{
	if (exec_cwd(cmd, path))
		return (1);
	if (cmd && *cmd && access(*path, F_OK) == 0)
	{
		if (is_a_directory(*path))
			return (free(*path), print_err(126, 2,
					"my(s)hell: %s: Is a directory\n", cmd), 126);
		if (access(*path, X_OK) == 0)
			return (0);
		else
			return (free(*path), print_err(126, 2,
					"my(s)hell: %s: Permission denied\n", cmd), 126);
	}
	else
		return (free(*path), print_err(127, 2,
				"my(s)hell: %s: No such file or directory\n", cmd), 127);
}

int	get_cwd(char *cmd, char **path, char **path_array)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_err(1, 2, "my(s)hell: malloc error in getcwd function\n"),
			1);
	path_array[0] = cwd;
	if (get_exec(path_array, 0, cmd, path))
		return (free(cwd), free_array(path_array), g_last_exit_status);
	if (access(*path, F_OK | X_OK) == 0)
	{
		free(cwd);
		return (0);
	}
	free(cwd);
	cwd = NULL;
	*path = cmd;
	return (-1);
}

int	get_path(char *cmd, t_env *env, char **path)
{
	char	**path_array;
	char	*pre_path;

	pre_path = NULL;
	if (handle_initial_path(&pre_path, env))
		return (1);
	if (pre_path == NULL)
		return (exec_from_cwd(cmd, path));
	if (pre_path && pre_path[0] == '\0')
		return (free(pre_path), exec_from_cwd(cmd, path));
	path_array = ft_split(pre_path, ':');
	if (path_array == NULL)
		return (free(pre_path), print_err(1, 2,
				"my(s)hell: malloc error in split function\n"), 1);
	free(pre_path);
	if (path_array[0] == NULL)
	{
		if (get_cwd(cmd, path, path_array))
			return (free_array(path_array), -1);
		return (free_array(path_array), 0);
	}
	return (exec_from_path(path_array, cmd, path));
}
