/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:57 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/08 17:47:25 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	question_mark(char **start, char *buffer, char *dollar,
		int dollar_status)
{
	char	*exit_status;

	exit_status = ft_itoa(dollar_status);
	if (exit_status == NULL)
		return (print_err(1, 2, "my(s)hell: malloc in ft_itoa\n"), 1);
	if (!check_null(exit_status, &g_last_exit_status))
		return (1);
	ft_strcat(buffer, exit_status);
	free(exit_status);
	*start = dollar + 2;
	return (0);
}

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
	var_end = ft_strpbrk(var_start, " \t\n\"'$/.:?");
	if (var_end == NULL)
		var_end = var_start + ft_strlen(var_start);
	ft_strncpy(var_name, var_start, var_end - var_start);
	if (var_end - var_start > 4096)
	{
		print_err(1, 2, "my(s)hell: too long input\n");//contol!
				g_last_exit_status = 1;
				return (1);
	}
	var_name[var_end - var_start] = '\0';
	var_value = get_env_var(*env, var_name);
	if (var_value != NULL)
	{
		while (ft_strlcat(l_vars->buffer, var_value, l_vars->buffer_size) >= l_vars->buffer_size)
		{
			//int original_size = ft_strlen(l_vars->buffer);
			size_t new_buf_size = l_vars->buffer_size * 2;
			char *new_buffer = ft_realloc(l_vars->buffer, l_vars->buffer_size, new_buf_size);
			if (new_buffer == NULL)
			{
				print_err(1, 2, "my(s)hell: malloc error 8\n");
				g_last_exit_status = 1;
				return (1);
			}
			l_vars->buffer = new_buffer;
			l_vars->buffer_size = new_buf_size;
		}
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
//int	handle_dollar_sign(char **start, char *buffer, t_free_data *exec_data,
			//int buffer_size)
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
	while (dollar != NULL)
	{
		ft_strncat(l_vars->buffer, *start, dollar - *start);
		if (*(dollar + 1) == '?')
		{
			if (question_mark(start, l_vars->buffer, dollar, exec_data->dollar_status))
				return (1);
		}
		else if (*(dollar + 1) == '\0' || ft_strchr("$ \"'/", *(dollar + 1)))
			handle_special_chars(start, l_vars->buffer, dollar);
		else if (handle_var_name(start, l_vars, dollar, &exec_data->env))
			return (1);
		dollar = ft_strchr((dollar + 1), '$');
	}
	dollar = ft_strchr(start_store, '$');
	if_dollar(l_vars->buffer, start_store, dollar, start);
	return (0);
}
