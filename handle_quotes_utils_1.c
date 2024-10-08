/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:00 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 11:46:24 by dpadenko         ###   ########.fr       */
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

	if (updated_result == NULL)
		return (0);
	new_result = ft_strjoin(*result, updated_result);
	if (!check_null(new_result, &vars->error))
		return (print_err(1, 2, "my(s)hell: malloc error 19\n"), 1);
	free(*result);
	*result = new_result;
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
		return (print_err(1, 2, "my(s)hell: malloc error 20\n"));
}

int	handle_dollar_error(char **token, t_handle_vars *l_vars, t_exec_vars *vars,
		t_free_data *exec_data)
{
	if (handle_dollar_sign(token, l_vars, exec_data))
	{
		vars->error = 1;
		return (1);
	}
	return (0);
}
