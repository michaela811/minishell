/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:37:18 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_free_data	*init_command_data(char **envp)
{
	t_free_data	*exec_data;

	exec_data = malloc(sizeof(t_free_data));
	if (!exec_data)
		return (NULL);
	exec_data->env = init_environment(envp);
	if (exec_data->env == NULL)
	{
		print_err(1, 2, "init_environment failed\n");
		free(exec_data);
		return (NULL);
	}
	exec_data->token_list = NULL;
	exec_data->environment = NULL;
	exec_data->tree = NULL;
	exec_data->token_list_start = NULL;
	exec_data->tree_start = NULL;
	return (exec_data);
}

void	init_handle_vars(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->result = malloc(sizeof(char *));//ADD MEMORY CHECK
	*l_vars->result = ft_strdup("");//ADD MEMORY CHECK
	//l_vars->current = &vars->args[vars->i];
	l_vars->current = malloc(sizeof(char *));//ADD MEMORY CHECK
	*l_vars->current = ft_strdup(vars->args[vars->i]);

	//ADD MEMORY CHECK
	l_vars->delimiters = "'\"";
	l_vars->current_start = *l_vars->current;
	//l_vars->current_start = malloc(sizeof(char *)); // Allocate memory for current_start
    //*l_vars->current_start = *l_vars->current; //
}

void	init_handle_quote_redirect(t_handle_vars *l_vars, t_p_tree **node)
{
	l_vars->result = malloc(sizeof(char *));
	*l_vars->result = ft_strdup("");
	l_vars->current = &(*node)->child->data->lexeme;
	l_vars->delimiters = "'\"";
}
