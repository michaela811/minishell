/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_redirect.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 11:08:21 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data, int *here_docs)
{
	if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars, here_docs, exec_data));
	if ((*node)->data->type == RED_FROM)
		return (handle_redirection_from(node, vars, exec_data));
	else if ((*node)->data->type == RED_TO)
		return (handle_redirection_to(node, vars, exec_data));
	else if ((*node)->data->type == APPEND)
		return (handle_redirection_append(node, vars, exec_data));
	return (g_last_exit_status);
}

int	handle_redirection_from(t_p_tree **node,
			t_exec_vars *vars, t_free_data *exec_data)
{
	char	*saved_lexeme;

	saved_lexeme = ft_strdup((*node)->child->data->lexeme);
	if (!saved_lexeme)
		return (print_err(1, 2, "my(s)hell: malloc error 4\n"), 1);
	quotes_glob_redirect(node, vars, exec_data);
	if (vars->error)
		return (g_last_exit_status);
	if (is_ambiguous_redirect(node, vars, saved_lexeme))
		return (g_last_exit_status);
	free(saved_lexeme);
	if (vars->fd_in && vars->fd_in != -1)
		close(vars->fd_in);
	vars->fd_in = open((*node)->child->data->lexeme, O_RDONLY);
	if (vars->fd_in == -1)
	{
		vars->error = 1;
		print_err(1, 2, "my(s)hell: %s: No such file or directory\n",
			(*node)->child->data->lexeme);
		return (g_last_exit_status);
	}
	*node = (*node)->child;
	return (0);
}

int	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data)
{
	char	*saved_lexeme;

	saved_lexeme = ft_strdup((*node)->child->data->lexeme);
	if (!saved_lexeme)
		return (print_err(1, 2, "my(s)hell: malloc error 5\n"), 1);
	quotes_glob_redirect(node, vars, exec_data);
	if (g_last_exit_status)
		return (g_last_exit_status);
	if (is_ambiguous_redirect(node, vars, saved_lexeme))
		return (g_last_exit_status);
	free(saved_lexeme);
	if (vars->fd_out != 1 && vars->fd_out != -1)
		close(vars->fd_out);
	vars->fd_out = open((*node)->child->data->lexeme,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (vars->fd_out == -1)
	{
		redirect_to_error(vars, node);
		return (g_last_exit_status);
	}
	*node = (*node)->child;
	return (0);
}

int	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data)
{
	char	*saved_lexeme;
	char	*exp_lexeme;

	saved_lexeme = ft_strdup((*node)->child->data->lexeme);
	if (!saved_lexeme)
		return (print_err(1, 2, "my(s)hell: malloc error 6\n"), 1);
	quotes_glob_redirect(node, vars, exec_data);
	if (g_last_exit_status)
		return (g_last_exit_status);
	if (is_ambiguous_redirect(node, vars, saved_lexeme))
		return (g_last_exit_status);
	free(saved_lexeme);
	exp_lexeme = malloc(4096);
	if (!exp_lexeme)
		return (redirect_append_error(vars), g_last_exit_status);
	ft_memset(exp_lexeme, '\0', sizeof(exp_lexeme));
	ft_strcpy(exp_lexeme, (*node)->child->data->lexeme);
	if (helper_is_dir(exp_lexeme, vars))
		return (g_last_exit_status);
	free(exp_lexeme);
	if (vars->fd_out != 1 && vars->fd_out != -1)
		close(vars->fd_out);
	vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT
			| O_APPEND, 0644);
	if (helper_fd_out_checker(node, vars))
		return (g_last_exit_status);
	*node = (*node)->child;
	return (0);
}

int	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars,
		int *here_docs, t_free_data *exec_data)
{
	char	*filename;

	if (*here_docs != -1)
		vars->fd_in = *here_docs;
	else
	{
		if (vars->fd_in && vars->fd_in != -1)
			close(vars->fd_in);
		filename = handle_here_doc(node, vars, exec_data);
		if (g_last_exit_status)
			return (g_last_exit_status);
		vars->fd_in = open(filename, O_RDONLY);
		if (vars->fd_in == -1)
			return (print_err(1, 2, "open"), 1);
	}
	*node = (*node)->child;
	return (0);
}
