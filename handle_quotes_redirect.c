/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_redirect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:54 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:36:56 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quotes_glob_redirect(t_p_tree **node, t_exec_vars *vars, t_env **env)
{
	t_handle_vars	l_vars;

	init_handle_quote_redirect(&l_vars, node);
	vars->error = 0;
	while (*l_vars.current != NULL && **l_vars.current != '\0')
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(l_vars.current, l_vars.result, vars, env);
		else
			handle_no_quotes_redirect(&l_vars, vars, env, node);
		if (vars->end)
		{
			vars->end = 0;
			g_last_exit_status = 0;
			return (free_handle_vars(&l_vars));
		}
		if (vars->error)
			break ;
	}
	if (!vars->error)
		return (handle_quotes_final_assign(&(*node)->child->data->lexeme,
				l_vars.result, vars));
	return (free_handle_vars(&l_vars));
}

void	handle_no_current_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
			sizeof(l_vars->buffer));
		if (g_last_exit_status)
		{
			vars->error = 1;
			return ;
		}
		*l_vars->result = ft_strjoin(*l_vars->result, l_vars->buffer);
		if (!check_null(*l_vars->result, &vars->error))
			return ;
		(*node)->child->data->lexeme = ft_strdup(*l_vars->result);
	}
	else
	{
		(*node)->child->data->lexeme = ft_strjoin(*l_vars->result,
				l_vars->token);
		if (!check_null((*node)->child->data->lexeme, &vars->error))
			return ;
	}
	vars->end = 1;
}

void	handle_with_current_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
{
	char	delimiter;

	delimiter = **l_vars->current;
	**l_vars->current = '\0';
	if (ft_strchr((*node)->data->lexeme, '$') != NULL)
	{
		handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
			sizeof(l_vars->buffer));
		if (g_last_exit_status)
		{
			vars->error = 1;
			return ;
		}
	}
	else
	{
		*l_vars->result = ft_strjoin(*l_vars->result, l_vars->token);
		if (!check_null(*l_vars->result, &vars->error))
			return ;
	}
	**l_vars->current = delimiter;
	if (**l_vars->current == '\'')
		vars->inside_single_quotes = 1;
	else if (**l_vars->current == '\"')
		vars->inside_double_quotes = 1;
	(*l_vars->current)++;
}

void	handle_no_quotes_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
{
	l_vars->token = *l_vars->current;
	*l_vars->current = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (*l_vars->current == NULL)
	{
		handle_no_current_redirect(l_vars, vars, env, node);
		return ;
	}
	handle_with_current_redirect(l_vars, vars, env, node);
}
