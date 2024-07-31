/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 20:04:49 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes_glob(t_p_tree **node, t_exec_vars *vars,
		t_env **env)
{
	t_handle_vars	l_vars;

	if (init_handle_vars(&l_vars, vars))
		vars->error = 1;
	//if (vars->error)
		//return (free_handle_vars(&l_vars));
	//vars->error = 0;
	while (**l_vars.current != '\0' && !vars->error)
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(l_vars.current, l_vars.result, vars,
				env);
		else
			handle_no_quotes(&l_vars, vars, env, node);
		if (vars->end)
		{
			vars->end = 0;
			*l_vars.current = l_vars.current_start;
			return (free_handle_vars(&l_vars));
		}
		if (vars->error)
			break ;
	}
	*l_vars.current = l_vars.current_start;
	handle_error_and_free(vars, &l_vars);
}

void	handle_single_quotes(char **current, char **result, t_exec_vars *vars)
{
	char	*token;

	token = *current;
	*current = ft_strchr(*current, '\'');
	if (!check_null(*current, &vars->error))
		return ;
	**current = '\0';
	if (update_result(result, token, vars))
		return ;
	vars->inside_single_quotes = 0;
	(*current)++;
	if (!check_null(*current, &vars->end))
		return ;
}

void	handle_double_quotes_split(char **current, t_exec_vars *vars)
{
	vars->inside_double_quotes = 0;
	(*current)++;
}

void	handle_double_quotes(char **current, char **result,
			t_exec_vars *vars, t_env **env)
{
	char	buffer[1024];
	char	*token;

	ft_memset(buffer, '\0', sizeof(buffer));
	token = *current;
	*current = ft_strchr(*current, '\"');
	if (!check_null(*current, &vars->error))
		return ;
	**current = '\0';
	if (ft_strchr(token, '$') != NULL)
	{
		if (handle_dollar_error(&token, buffer, vars, env))
			return ;
		if(update_result(result, buffer, vars))
			return ;
	}
	else if (update_result(result, token, vars))
			return ;
	handle_double_quotes_split(current, vars);
	if (!check_null(*current, &vars->end))
		return ;
}

void	handle_no_quotes(t_handle_vars *l_vars, t_exec_vars *vars,
				t_env **env, t_p_tree **node)
{
	char *temp;

	l_vars->token = *l_vars->current;
	temp = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (temp == NULL)
	{
		handle_no_current(l_vars, vars, env, node);
		return ;
	}
	*l_vars->current = temp;
	handle_with_current(l_vars, vars, env, node);
}
