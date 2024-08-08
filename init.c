/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/08 17:19:48 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_free_data	*init_command_data(char **envp, t_free_data *exec_data)
{
	exec_data->env = init_environment(envp);
	exec_data->token_list = NULL;
	exec_data->environment = NULL;
	exec_data->tree = NULL;
	exec_data->token_list_start = NULL;
	exec_data->tree_start = NULL;
	exec_data->hd_fd = -1;
	exec_data->dollar_status = 0;
	return (exec_data);
}

int	init_handle_vars(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->buffer_size = 4096;
	l_vars->buffer = malloc(l_vars->buffer_size);
	if (!l_vars->buffer)
		return (print_err(1, 2, "my(s)hell: malloc error xx\n"), 1);
	l_vars->result = malloc(sizeof(char *));
	if (!l_vars->result)
		return (print_err(1, 2, "my(s)hell: malloc error 21\n"), 1);
	*l_vars->result = ft_strdup("");
	if (!*l_vars->result)
		return (print_err(1, 2, "my(s)hell: malloc error 22\n"), 1);
	l_vars->current = malloc(sizeof(char *));
	if (!l_vars->current)
		return (print_err(1, 2, "my(s)hell: malloc error 23\n"), 1);
	*l_vars->current = ft_strdup(vars->args[vars->i]);
	if (!*l_vars->current)
		return (print_err(1, 2, "my(s)hell: malloc error 24\n"), 1);
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	return (0);
}

int	init_handle_quote_redirect(t_handle_vars *l_vars, t_p_tree **node)
{
	l_vars->buffer_size = 4096;
	l_vars->buffer = malloc(l_vars->buffer_size);
	if (!l_vars->buffer)
		return (print_err(1, 2, "my(s)hell: malloc error xx\n"), 1);
	l_vars->result = malloc(sizeof(char *));
	if (!l_vars->result)
		return (print_err(1, 2, "my(s)hell: malloc error 25\n"), 1);
	*l_vars->result = ft_strdup("");
	if (!*l_vars->result)
		return (free(l_vars->result), print_err(1, 2,
				"my(s)hell: malloc error 26\n"), 1);
	l_vars->current = malloc(sizeof(char *));
	if (!l_vars->current)
		return (free(*l_vars->result), free(l_vars->result),
			print_err(1, 2, "my(s)hell: malloc error 27\n"), 1);
	*l_vars->current = ft_strdup((*node)->child->data->lexeme);
	if (!*l_vars->current)
		return (free(l_vars->current), free(*l_vars->result),
			free(l_vars->result),
			print_err(1, 2, "my(s)hell: malloc error 28\n"), 1);
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	return (0);
}
