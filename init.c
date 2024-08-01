/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 10:50:40 by mmasarov         ###   ########.fr       */
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
	return (exec_data);
}

int	init_handle_vars(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->result = malloc(sizeof(char *));
	if (!l_vars->result)
		return (1);
	*l_vars->result = ft_strdup("");
	if (!*l_vars->result)
		return (1);
	l_vars->current = malloc(sizeof(char *));
	if (!l_vars->current)
		return (1);
	*l_vars->current = ft_strdup(vars->args[vars->i]);
	if (!*l_vars->current)
		return (1);
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	return (0);
}

int	init_handle_quote_redirect(t_handle_vars *l_vars, t_p_tree **node)
{
	l_vars->result = malloc(sizeof(char *));
	if (!l_vars->result)
		return (1);
	*l_vars->result = ft_strdup("");
	if (!*l_vars->result)
		return (free(l_vars->result), 1);
	l_vars->current = malloc(sizeof(char *));
	if (!l_vars->current)
		return (free(*l_vars->result), free(l_vars->result), 1);
	*l_vars->current = ft_strdup((*node)->child->data->lexeme);
	if (!*l_vars->current)
		return (free(l_vars->current), free(*l_vars->result),
			free(l_vars->result), 1);
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	return (0);
}
