/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_redirect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:54 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 12:13:52 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quotes_glob_redirect(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data)
{
	t_handle_vars	l_vars;

	if (init_handle_quote_redirect(&l_vars, node))
		vars->error = 1;
	if (vars->error)
		return ;
	while (*l_vars.current != NULL && **l_vars.current != '\0')
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(l_vars.current, l_vars.result, vars, exec_data);
		else
			handle_no_quotes_redirect(&l_vars, vars, exec_data, node);
		if (vars->end)
		{
			vars->end = 0;
			//g_last_exit_status = 0;
			return (free_handle_vars(&l_vars));
		}
		if (vars->error)
			break ;
	}
	*l_vars.current = l_vars.current_start;
	handle_error_and_free_redirect(vars, &l_vars, node);
}

void	handle_no_current_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data, t_p_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		if (handle_dollar_error(&l_vars->token, l_vars->buffer, vars, exec_data))
			return ;
		if (update_result(l_vars->result, l_vars->buffer, vars))
			return ;
		free((*node)->child->data->lexeme);
		(*node)->child->data->lexeme = ft_strdup(*l_vars->result);
		if (!check_null((*node)->child->data->lexeme, &vars->error))
			return ;
	}
	else
	{
		free((*node)->child->data->lexeme);
		(*node)->child->data->lexeme = ft_strjoin(*l_vars->result,
				l_vars->token);
		if (!check_null((*node)->child->data->lexeme, &vars->error))
			return ;
	}
	vars->end = 1;
}

void	handle_with_current_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data, t_p_tree **node)
{
	char	delimiter;

	delimiter = **l_vars->current;
	**l_vars->current = '\0';
	if (ft_strchr((*node)->data->lexeme, '$') != NULL)
	{
		if (handle_dollar_sign(&l_vars->token, l_vars->buffer, exec_data,
				sizeof(l_vars->buffer)))
		{
			vars->error = 1;
			return ;
		}
	}
	else if (update_result(l_vars->result, l_vars->token, vars))
		return ;
	**l_vars->current = delimiter;
	if (**l_vars->current == '\'')
		vars->inside_single_quotes = 1;
	else if (**l_vars->current == '\"')
		vars->inside_double_quotes = 1;
	(*l_vars->current)++;
}

void	handle_no_quotes_redirect(t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data, t_p_tree **node)
{
	char	*temp;

	l_vars->token = *l_vars->current;
	temp = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (temp == NULL)
	{
		handle_no_current_redirect(l_vars, vars, exec_data, node);
		return ;
	}
	*l_vars->current = temp;
	handle_with_current_redirect(l_vars, vars, exec_data, node);
}

void	handle_error_and_free_redirect(t_exec_vars *vars, t_handle_vars *l_vars,
		t_p_tree **node)
{
	if (!vars->error)
	{
		free((*node)->child->data->lexeme);
		(*node)->child->data->lexeme = NULL;
		handle_quotes_final_assign(&(*node)->child->data->lexeme,
			l_vars->result, vars);
		free_handle_vars(l_vars);
		return ;
	}
	free_handle_vars(l_vars);
}
