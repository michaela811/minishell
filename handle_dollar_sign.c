/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:57 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 15:16:39 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	question_mark(char **start, char *buffer, char *dollar,
		int dollar_status)
{
	char	*exit_status;

	exit_status = ft_itoa(dollar_status);
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

void	handle_var_name(char **start, char *buffer, char *dollar, t_env **env)
{
	char	var_name[1024];
	char	*var_start;
	char	*var_end;
	char	*var_value;

	ft_memset(var_name, '\0', sizeof(var_name));
	var_start = dollar + 1;
	var_end = ft_strpbrk(var_start, " \t\n\"'$/.:?");
	if (var_end == NULL)
		var_end = var_start + ft_strlen(var_start);
	ft_strncpy(var_name, var_start, var_end - var_start);
	var_name[var_end - var_start] = '\0';
	var_value = get_env_var(*env, var_name);
	if (var_value != NULL)
		ft_strcat(buffer, var_value);
	*start = var_end;
}

void	if_dollar(char *buffer, char *start_store, char *dollar, char **start)
{
	if (dollar == NULL)
		ft_strcpy(buffer, start_store);
	else if (*start)
		ft_strcat(buffer, *start);
}

int	handle_dollar_sign(char **start, char *buffer, t_free_data *exec_data,
			int buffer_size)
{
	char	*dollar;
	char	*start_store;

	if (start == NULL || *start == NULL)
		return (0);
	ft_memset(buffer, '\0', buffer_size);
	start_store = *start;
	dollar = ft_strchr(*start, '$');
	while (dollar != NULL)
	{
		ft_strncat(buffer, *start, dollar - *start);
		if (*(dollar + 1) == '?')
		{
			if (question_mark(start, buffer, dollar, exec_data->dollar_status))
				return (1);
		}
		else if (*(dollar + 1) == '\0' || ft_strchr("$ \"'/", *(dollar + 1)))
			handle_special_chars(start, buffer, dollar);
		else
			handle_var_name(start, buffer, dollar, &exec_data->env);
		dollar = ft_strchr((dollar + 1), '$');
	}
	dollar = ft_strchr(start_store, '$');
	if_dollar(buffer, start_store, dollar, start);
	return (0);
}
