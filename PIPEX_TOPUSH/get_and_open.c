/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:43:34 by mmasarov          #+#    #+#             */
/*   Updated: 2024/01/30 17:21:39 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_file(char *file, int read_write)
{
	int	fd;

	if (read_write == 0)
		fd = open(file, O_RDONLY, 0000444);
	if (read_write == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0000644);
	if (fd == -1)
		error_message("Error in FD: ");
	return (fd);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**path;
	char	**s_cmd;

	i = -1;
	path = ft_split(get_env(env, "PATH"), ':');
	if (path == NULL)
		error_message("Malloc error in split function");
	s_cmd = ft_split(cmd, ' ');
	if (s_cmd == NULL)
		error_message("Malloc error in split function");
	while (path[++i])
	{
		exec = get_exec(path, i, s_cmd);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_array(s_cmd);
			return (exec);
		}
		free(exec);
	}
	free_array(path);
	free_array(s_cmd);
	return (cmd);
}

char	*get_env(char **env, char *var)
{
	int		i;
	int		j;
	char	*s_var;

	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		s_var = ft_substr(env[i], 0, j);
		if (s_var == NULL)
			error_message("Malloc error in substr function");
		if (ft_strcmp(s_var, var) == 0)
		{
			free(s_var);
			return (env[i] + j + 1);
		}
		free(s_var);
	}
	return (NULL);
}

char	*get_exec(char **path, int i, char **s_cmd)
{
	char	*exec;
	char	*path_part;

	path_part = ft_strjoin(path[i], "/");
	if (path_part == NULL)
		error_message("Malloc error in strjoin function");
	exec = ft_strjoin(path_part, s_cmd[0]);
	if (exec == NULL)
		error_message("Malloc error in strjoin function");
	free(path_part);
	return (exec);
}
