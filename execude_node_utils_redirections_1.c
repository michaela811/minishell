/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execude_node_utils_redirections_1.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:18 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 15:56:21 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_redirection_from(t_p_tree **node,
			t_exec_vars *vars, t_env **env)
{
	quotes_glob_redirect(node, vars, env);
	if (vars->error)
		return ;
	vars->fd_in = open((*node)->child->data->lexeme, O_RDONLY);
	if (vars->fd_in == -1)
	{
		vars->error = 1;
		print_err(1, 2, "my(s)hell: %s: No such file or directory\n",
			(*node)->child->data->lexeme);
	}
	*node = (*node)->child;
}

void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_env **env)
{
	//(*node)->child->data->lexeme = handle_quotes_echo
		//((*node)->child->data->lexeme, &vars->error);
	quotes_glob_redirect(node, vars, env);
	if (g_last_exit_status)
		return ;
	//printf("node->child->data->lexeme: %s\n", (*node)->child->data->lexeme);
	vars->fd_out = open((*node)->child->data->lexeme,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (vars->fd_out == -1)
	{
		vars->error = 1;
		if (errno == EACCES)
			print_err(1, 2, "my(s)hell: %s: Permission denied\n",
				(*node)->child->data->lexeme);
		else
			print_err(1, 2,
				"my(s)hell: %s: No such file or directory\n",
				(*node)->child->data->lexeme);
	}
	*node = (*node)->child;
}
