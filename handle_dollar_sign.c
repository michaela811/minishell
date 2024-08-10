/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:57 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 11:40:21 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_special_chars(char **start, char *buffer, char *dollar)
{
	ft_strcat(buffer, "$");
	*start = dollar + 1;
}

int	handle_var_name(char **start, t_handle_vars *l_vars, char *dollar,
		t_env **env)
{
	char	var_name[4096];
	char	*var_start;
	char	*var_end;
	char	*var_value;

	ft_memset(var_name, '\0', 4096);
	var_start = dollar + 1;
	var_end = find_first_non_alnum(var_start);
	if (var_end == NULL)
		var_end = var_start + ft_strlen(var_start);
	ft_strncpy(var_name, var_start, var_end - var_start);
	var_name[var_end - var_start] = '\0';
	var_value = get_env_var(*env, var_name);
	if (var_value != NULL)
	{
		while (ft_strlcat_custom(l_vars->buffer, var_value,
				l_vars->buffer_size) == -1)
			if (resize_buffer(&l_vars->buffer, &l_vars->buffer_size))
				return (1);
	}
	*start = var_end;
	return (0);
}

void	if_dollar(char *buffer, char *start_store, char *dollar, char **start)
{
	if (dollar == NULL)
		ft_strcpy(buffer, start_store);
	else if (*start)
		ft_strcat(buffer, *start);
}

int	process_dollar_signs(char **start, char *dollar, t_handle_vars *l_vars,
	t_free_data *exec_data)
{
	while (dollar != NULL)
	{
		while (dollar - *start >= l_vars->buffer_size)
		{
			if (resize_buffer(&l_vars->buffer, &l_vars->buffer_size))
				return (1);
		}
		ft_strncat_custom(l_vars->buffer, *start, dollar - *start);
		if (*(dollar + 1) == '?')
		{
			if (question_mark(start, l_vars->buffer, dollar,
					exec_data->dollar_status))
				return (1);
		}
		else if (*(dollar + 1) == '\0' || ft_strchr("$ \"'/", *(dollar + 1)))
			handle_special_chars(start, l_vars->buffer, dollar);
		else if (handle_var_name(start, l_vars, dollar, &exec_data->env))
			return (1);
		dollar = ft_strchr((dollar + 1), '$');
	}
	return (0);
}

int	handle_dollar_sign(char **start, t_handle_vars *l_vars,
		t_free_data *exec_data)
{
	char	*dollar;
	char	*start_store;

	if (start == NULL || *start == NULL)
		return (0);
	ft_memset(l_vars->buffer, '\0', l_vars->buffer_size);
	start_store = *start;
	dollar = ft_strchr(*start, '$');
	if (process_dollar_signs(start, dollar, l_vars, exec_data))
		return (1);
	dollar = ft_strchr(start_store, '$');
	if_dollar(l_vars->buffer, start_store, dollar, start);
	return (0);
}
