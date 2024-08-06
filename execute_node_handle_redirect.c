/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_redirect.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 11:42:23 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data, int *here_docs)
{
	if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars, here_docs, exec_data));
	if ((*node)->data->type == RED_FROM)
		handle_redirection_from(node, vars, exec_data);
	else if ((*node)->data->type == RED_TO)
		handle_redirection_to(node, vars, exec_data);
	else if ((*node)->data->type == APPEND)
		handle_redirection_append(node, vars, exec_data);
}

int	is_ambiguous_redirect(t_p_tree **node, t_exec_vars *vars, char *saved_lexeme)
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

void	handle_redirection_from(t_p_tree **node,
			t_exec_vars *vars, t_free_data *exec_data)
{
	char *saved_lexeme;

	saved_lexeme = ft_strdup((*node)->child->data->lexeme);
	quotes_glob_redirect(node, vars, exec_data);
	if (vars->error)
		return ;
	if (is_ambiguous_redirect(node, vars, saved_lexeme))
		return ;
	free(saved_lexeme);
	if (vars->fd_in)
		close(vars->fd_in);
	vars->fd_in = open((*node)->child->data->lexeme, O_RDONLY);
	if (vars->fd_in == -1)
	{
		vars->error = 1;
		print_err(1, 2, "my(s)hell: %s: No such file or directory\n",
			(*node)->child->data->lexeme);
		return ;
	}
	*node = (*node)->child;
}

void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data)
{
	char* saved_lexeme;

	saved_lexeme = ft_strdup((*node)->child->data->lexeme);
	quotes_glob_redirect(node, vars, exec_data);
	if (g_last_exit_status)
		return ;
	if (is_ambiguous_redirect(node, vars, saved_lexeme))
		return ;
	free(saved_lexeme);
	if (vars->fd_out != 1)
		close(vars->fd_out);
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
		return ;
	}
	*node = (*node)->child;
}

void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data)
{
	//char	*start;
	char	*saved_lexeme;
	char	*exp_lexeme;

	saved_lexeme = ft_strdup((*node)->child->data->lexeme);
	quotes_glob_redirect(node, vars, exec_data);
	if (g_last_exit_status)
		return ;
	if (is_ambiguous_redirect(node, vars, saved_lexeme))
		return ;
	free(saved_lexeme);
	exp_lexeme = malloc(4096);
	if (!exp_lexeme)
	{
		print_err(1, 2, "my(s)hell: malloc\n");
		vars->error = 1;
		return ;
	}
	ft_memset(exp_lexeme, '\0', sizeof(exp_lexeme));
	//start = (*node)->child->data->lexeme;
	//if (handle_dollar_sign(&start, exp_lexeme, env, sizeof(exp_lexeme)))// do we need it???
		//return (free(exp_lexeme));
	ft_strcpy(exp_lexeme, (*node)->child->data->lexeme);
	if (helper_is_dir(exp_lexeme, vars))
		return ;
	free(exp_lexeme);
	if (vars->fd_out != 1)
		close(vars->fd_out);
	vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT
			| O_APPEND, 0644);
	helper_fd_out_checker(node, vars);
	*node = (*node)->child;
	//vars->i++;
}

void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars ,
		int *here_docs, t_free_data *exec_data)
{
	char	*filename;

	if (*here_docs != -1)
		vars->fd_in = *here_docs;
	else
	{
		if (vars->fd_in)
			close(vars->fd_in);
		filename = handle_here_doc(node, vars, exec_data);
		if (vars->error)
			return ;
		vars->fd_in = open(filename, O_RDONLY);
		if (vars->fd_in == -1)
		{
			print_err(1, 2, "open");
			vars->error = 1;
		}
	}
	*node = (*node)->child;
}
