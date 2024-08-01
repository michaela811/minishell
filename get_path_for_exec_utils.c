/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_for_exec_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:37:53 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	exec_cwd(char *cmd, char **exec)
{
	char	cwd[1024];
	char	*tmp;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		tmp = ft_strjoin(cwd, "/");
		if (tmp == NULL)
			return (print_err(1, 2,
					"my(s)hell: malloc error in strjoin function\n"), 1);
		*exec = ft_strjoin(tmp, cmd);
		if (*exec == NULL)
			return (free(tmp), print_err(1, 2,
					"my(s)hell: malloc error in strjoin function\n"), 1);
	}
	else
		return (print_err(1, 2,
				"my(s)hell: getcwd() error\n"), 1);
	return (free(tmp), 0);
}

int	exec_from_path(char **path_array, char *cmd, char **path)//exec_from_path(path_array, cmd, path)
{
	int	i;

	i = -1;
	while (path_array[++i])
	{
		if (get_exec(path_array, i, cmd, path))
			return (free_array(path_array), g_last_exit_status);
		if (access(*path, F_OK | X_OK) == 0)
			return (free_array(path_array), 0);
		if (!path_array[i + 1] && ft_strcmp(path_array[i], getenv("HOME")) == 0)
			return (free_array(path_array), -2);
		free(*path);
	}
	free_array(path_array);
	*path = cmd;
	return (-1);
}
