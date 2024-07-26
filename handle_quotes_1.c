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
	/* update_result(result, token, vars);
		if (vars->error)
			return ; */
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
		/* handle_dollar_sign(&token, buffer, env, sizeof(buffer));
		if (g_last_exit_status)
		{
			vars->error = 1;
			return ;
		} */
		if(update_result(result, buffer, vars))
			return ;
		/* update_result(result, buffer, vars);
		if (vars->error)
			return ; */
	}
	else if (update_result(result, token, vars))
			return ;
	/* else
	{
		update_result(result, token, vars);
		if (vars->error)
			return ;
	} */
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

/* int	remove_spaces(char **result, char *buffer, t_exec_vars *vars)
{
	char	*buffer_no_spaces;
	int		j;
	int		k;

	j = 0;
	k = 0;
	buffer_no_spaces = malloc(sizeof(char) * (ft_strlen(buffer) + 1));
	if (!check_null(buffer_no_spaces, &vars->error))
		return (1);
	while (buffer[j])
	{
		if (buffer[j] == ' ')
		{
			buffer_no_spaces[k++] = buffer[j++];
			while (buffer[j] == ' ')
				j++;
		}
		if (buffer[j] != '\0')
			buffer_no_spaces[k++] = buffer[j++];
	}
	buffer_no_spaces[k] = '\0';
	//free(buffer);
	//buffer = buffer_no_spaces;
	if (update_result(result, buffer_no_spaces, vars))
		return (free(buffer_no_spaces), 1);
	return (free(buffer_no_spaces), 0);
} */

