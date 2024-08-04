/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_for_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/04 10:30:11 by dpadenko         ###   ########.fr       */
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

/* int	exec_from_cwd(char *cmd, char **path)
{
	if (exec_cwd(cmd, path))
		return (1);
	if (cmd && *cmd && access(*path, F_OK | X_OK) == 0)
	{
		if (is_a_directory(*path))
			return (print_err(126, 2, "my(s)hell: %s: Is a directory\n", cmd),
				126);
		return (0);
	}
	//return (-1);
	return (print_err(127, 2,
			"my(s)hell: %s: No such file or directory\n",
			cmd), 127);
} */

int	exec_from_cwd(char *cmd, char **path)
{
	if (exec_cwd(cmd, path))
		return (1);
	if (cmd && *cmd && access(*path, F_OK) == 0)
	{
		if (is_a_directory(*path))
			return (free(*path), print_err(126, 2, "my(s)hell: %s: Is a directory\n", cmd), 126);
		if (access(*path, X_OK) == 0)
			return (0);
		else
			return (free(*path), print_err(126, 2, "my(s)hell: %s: Permission denied\n", cmd), 126);
	}
	else
		return (free(*path), print_err(127, 2, "my(s)hell: %s: No such file or directory\n", cmd), 127);
}

int	get_cwd(char *cmd, char **path, char **path_array)//get_cwd(cmd, path, path_array)
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

int	get_path(char *cmd, t_env *env, char **path)//get_path(vars->args[0], *env, path
{
	char	**path_array;
	char	*pre_path;
	//int exec_result;

	pre_path = NULL;
	if (handle_initial_path(&pre_path, env))
		return (1);
	if (pre_path == NULL)
		return (exec_from_cwd(cmd, path));//, exec_no_path(cmd));
	if (pre_path && pre_path[0] == '\0')
		return (free(pre_path), exec_from_cwd(cmd, path));//, exec_no_path(cmd));
	path_array = ft_split(pre_path, ':');
	if (path_array == NULL)
		return (free(pre_path), print_err(1, 2, "my(s)hell: malloc error in split function\n"), 1);
	free(pre_path);
	if (path_array[0] == NULL)
	{
		if (get_cwd(cmd, path, path_array))
			return (free_array(path_array), -1);//return (free_array(path), -1); To be tested what to return
		return (free_array(path_array), 0);
	}
	return (exec_from_path(path_array, cmd, path));
}

/* int	get_path(char *cmd, t_env *env, char **exec)
{
	int		i;
	char	**path;
	char	*pre_path;

	pre_path = NULL;
	i = -1;
	if (handle_colon(&pre_path, env))
		return (print_err(1, 2,
				"malloc error in handle_colon function\n"), 1);
	if (pre_path == NULL)
	{
		if (exec_cwd(cmd, exec))
			return (1);
		if (access(*exec, F_OK | X_OK) == 0)
			return (0);
		return (-1);
	}
	path = ft_split(pre_path, ':');
	if (path == NULL)
		return (free(pre_path), print_err(1, 2,
				"malloc error in split function\n"), 1);
	free (pre_path);
	if (path[0] == NULL)
	{
		if(get_cwd(cmd, exec, path))
			return(free_array(path), -1);//return (free_array(path), -1); To be tested what to return
		return (free_array(path), 0);
	}
	while (path[++i])
	{
		if (get_exec(path, i, cmd, exec))
			return (free_array(path), g_last_exit_status);
		if (access(*exec, F_OK | X_OK) == 0)
			return (free_array(path), 0);
		if (!path[i + 1] && strcmp(path[i], getenv("HOME")) == 0)
			return (-2);
		free(*exec);
	}
	free_array(path);
	*exec = cmd;
	return (-1);
} */