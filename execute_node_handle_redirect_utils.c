/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_redirect_utils.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:03 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 14:00:30 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (-1);
	return (S_ISDIR(path_stat.st_mode));
}

int	helper_is_dir(char	*exp_lexeme, t_exec_vars *vars)
{
	int	is_dir;

	is_dir = is_directory(exp_lexeme);
	if (is_dir == 1)
	{
		vars->error = 1;
		print_err(1, 2, "my(s)hell: %s: Is a directory\n",
			exp_lexeme);
		free(exp_lexeme);
		return (1);
	}
	return (0);
}

int	helper_fd_out_checker(t_p_tree **node, t_exec_vars *vars)
{
	if (vars->fd_out == -1)
	{
		vars->error = 1;
		if (errno == EACCES)
			return (print_err(1, 2, "my(s)hell: %s: Permission denied\n",
				(*node)->child->data->lexeme), 1);
		else if (errno == ENOENT)
			return (print_err(1, 2,
					"my(s)hell: %s: No such file or directory\n",
				(*node)->child->data->lexeme), 1);
		else
			return (print_err(1, 2,
				"my(s)hell: %s: Not a directory\n",
				(*node)->child->data->lexeme), 1);
	}
	return (0);
}
