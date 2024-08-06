/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_redirect_error.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:09:46 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/06 16:11:14 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_ambiguous_redirect(t_p_tree **node, t_exec_vars *vars,
		char *saved_lexeme)
{
	if (ft_strcmp((*node)->child->data->lexeme, "") == 0)
	{
		vars->error = 1;
		return (print_err(1, 2,
				"my(s)hell: %s: ambiguous redirect\n",
				saved_lexeme), free(saved_lexeme), 1);
	}
	return (0);
}

void	redirect_to_error(t_exec_vars *vars, t_p_tree **node)
{
	vars->error = 1;
	if (errno == EACCES)
		print_err(1, 2, "my(s)hell: %s: Permission denied\n",
			(*node)->child->data->lexeme);
	else
		print_err(1, 2, "my(s)hell: %s: No such file or directory\n",
			(*node)->child->data->lexeme);
}

void	redirect_append_error(t_exec_vars *vars)
{
	print_err(1, 2, "my(s)hell: malloc error\n");
	vars->error = 1;
}
