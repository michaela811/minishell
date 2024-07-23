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

int exec_cwd(char *cmd, char **exec)
{
	char cwd[1024];
	char *tmp;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		tmp = ft_strjoin(cwd, "/");
		if (tmp == NULL)
			return (print_err(1, 2,
				"my(s)hell: malloc error in strjoin function\n"), 1);
		*exec = ft_strjoin(tmp, cmd);
		if (*exec == NULL)
			return (free(tmp),print_err(1, 2,
				"my(s)hell: malloc error in strjoin function\n"), 1);
	}
	else
		return (print_err(1, 2,
				"my(s)hell: getcwd() error\n"), 1);
	return (free(tmp), 0);
}

int exec_from_path(char **path, char *cmd, char **exec)
{
    int i;

	i = -1;
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
}