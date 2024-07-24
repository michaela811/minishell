/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_parse_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/18 17:22:12 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_p_tree **node, t_exec_vars *vars, t_env **env, t_hd_data *here_docs)
{
	if ((*node)->data->type == RED_FROM)
		return (handle_redirection_from(node, vars, env));
	else if ((*node)->data->type == RED_TO)
		return (handle_redirection_to(node, vars, env));
	else if ((*node)->data->type == APPEND)
		return (handle_redirection_append(node, vars, env));
	else if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars, here_docs, env));
}

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
		return ;
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
		return ;
	}
	*node = (*node)->child;
}

void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_env **env)
{
	char	*start;
	char	*expanded_lexeme;

	quotes_glob_redirect(node, vars, env);
	if (g_last_exit_status)
		return ;
	expanded_lexeme = malloc(4096);
	if (!expanded_lexeme)
	{
		print_err(1, 2, "my(s)hell: malloc\n");
		vars->error = 1;
		return ;
	}
	ft_memset(expanded_lexeme, '\0', sizeof(expanded_lexeme));
	start = (*node)->child->data->lexeme;
	if (handle_dollar_sign(&start, expanded_lexeme, env, sizeof(expanded_lexeme)))
	//if (g_last_exit_status)
		return (free(expanded_lexeme));
	if (helper_is_dir(expanded_lexeme, vars))
		return ;
	free(expanded_lexeme);
	vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT
			| O_APPEND, 0644);
	helper_fd_out_checker(node, vars);
	*node = (*node)->child;
	//vars->i++;
}

void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars ,
		t_hd_data *here_docs, t_env **env)
{
	char	*filename;

	if (here_docs && here_docs->fd != -1)
		vars->fd_in = here_docs->fd;
	else
	{
		filename = handle_here_doc(node, vars, env);
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
	//vars->i++;? we shouldn't increment i here?
}