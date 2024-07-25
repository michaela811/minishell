/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:00 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:32:58 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_null(void *pointer, int *error_flag)
{
	if (pointer == NULL)
	{
		*error_flag = 1;
		g_last_exit_status = 1;
		return (0);
	}
	return (1);
}

int	update_result(char **result, char *updated_result, t_exec_vars *vars)
{
	char	*new_result;

	new_result = ft_strjoin(*result, updated_result);
	//new_result = NULL;
	if (!check_null(new_result, &vars->error))
	{
		return (1);
	}
	//new_result = ft_strjoin(*result, updated_result);
	free(*result); // Free the old string
	*result = new_result; // Assign the new string
	//ft_memset(updated_result, '\0', ft_strlen(updated_result));
	return (0);
}

int	buffer_end_space(char *buffer)
{
	int	i;

	i = ft_strlen(buffer) - 1;
	if (buffer[i] == ' ')
		return (1);
	return (0);
}

void	update_args(t_exec_vars *vars, t_handle_vars *l_vars)
{
	free(vars->args[vars->i]);
	vars->args[vars->i] = ft_strjoin(*l_vars->result, l_vars->token);
	if (!check_null(vars->args[vars->i], &vars->error))
		return ;
}

int	handle_dollar_error(char **token, char *buffer, t_exec_vars *vars,
		t_env **env)
{
	if (handle_dollar_sign(token, buffer, env, sizeof(buffer)))
	//if (g_last_exit_status)
	{
		vars->error = 1;
		return (1);
	}
	return (0);
}
