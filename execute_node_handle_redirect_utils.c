/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_redirect_utils.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:03 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 11:20:30 by mmasarov         ###   ########.fr       */
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
		print_err(1, 2, "my(s)hell: %s: Not a directory\n",
			exp_lexeme);
		free(exp_lexeme);
		return (1);
	}
	return (0);
}

void	helper_fd_out_checker(t_p_tree **node, t_exec_vars *vars)
{
	if (vars->fd_out == -1)
	{
		vars->error = 1;
		if (errno == EACCES)
			print_err(1, 2, "my(s)hell: %s: Permission denied\n",
				(*node)->child->data->lexeme);
		else
			print_err(1, 2,
				"my(s)hell: %s: Not a directory\n",
				(*node)->child->data->lexeme);
	}
}
