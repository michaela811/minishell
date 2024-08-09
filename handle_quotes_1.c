/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/08 15:45:21 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	vars_set_null(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->buffer = NULL;
	l_vars->token = NULL;
	l_vars->delimiters = NULL;
	l_vars->current = NULL;
	l_vars->result = NULL;
	l_vars->current_start = NULL;
	if (init_handle_vars(l_vars, vars))
	{
		free_handle_vars(l_vars);
		vars->error = 1;
	}
	return ;
}

void	handle_quotes_glob(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data)
{
	t_handle_vars	l_vars;

	vars_set_null(&l_vars, vars);
	if (vars->error)
		return ;
	while (**l_vars.current != '\0' && !vars->error)
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(&l_vars, vars, exec_data);
		else
			handle_no_quotes(&l_vars, vars, exec_data, node);
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

/* typedef struct s_buf_quotes
{
	char	*buffer;
	int		buffer_size;
}	t_buf_quotes;

int	init_buf_quotes(t_buf_quotes *buf_quotes)
{
	buf_quotes->buffer_size = 40;
	buf_quotes->buffer = malloc(buf_quotes->buffer_size);
	if (!buf_quotes->buffer)
		return (print_err(1, 2, "my(s)hell: malloc error xx\n"), 1);
	return (0);
} */
//void	handle_double_quotes(char **current, char **result,
			//t_exec_vars *vars, t_free_data *exec_data, char *buffer)
void	handle_double_quotes(t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data)
{
	//char	buffer[1024];
	char			*token;

	ft_memset(l_vars->buffer, '\0', sizeof(l_vars->buffer));
	token = *(l_vars->current);
	*(l_vars->current) = ft_strchr(*(l_vars->current), '\"');
	if (!check_null(*(l_vars->current), &vars->error))
		return ;
	**(l_vars->current) = '\0';
	if (ft_strchr(token, '$') != NULL)
	{
		if (handle_dollar_error(&token, l_vars, vars, exec_data))
			return ;
		if (update_result(l_vars->result, l_vars->buffer, vars))
			return ;
	}
	else if (update_result(l_vars->result, token, vars))
		return ;
	handle_double_quotes_split(l_vars->current, vars);
	if (!check_null(*(l_vars->current), &vars->end))
		return ;
}

void	handle_no_quotes(t_handle_vars *l_vars, t_exec_vars *vars,
				t_free_data *exec_data, t_p_tree **node)
{
	char	*temp;

	l_vars->token = *l_vars->current;
	temp = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (temp == NULL)
	{
		handle_no_current(l_vars, vars, exec_data, node);
		return ;
	}
	*l_vars->current = temp;
	handle_with_current(l_vars, vars, exec_data, node);
}
