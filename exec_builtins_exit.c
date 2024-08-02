/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 10:27:24 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_exit(char *result, t_exec_vars *vars)
{
	if (!is_string_numeric(result) || overflow_check(result))
		return (print_err(2, 2,
				"my(s)hell: %s: %s: numeric argument required\n",
				vars->args[0], vars->args[1]), g_last_exit_status);
	if (result[0] == '\0' && ft_strlen(result) == 0)
		return (print_err(2, 2,
				"my(s)hell: %s: %s: numeric argument required\n",
				vars->args[0], vars->args[1]), g_last_exit_status);
	if (vars->args[2] != NULL)
		return (print_err(1, 2,
				"my(s)hell: %s: too many arguments\n",
				vars->args[0]), g_last_exit_status);
	else
		return (g_last_exit_status);
}

int	is_string_numeric(const char *str)
{
	while (ft_isspace((unsigned char)*str))
		str++;
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (ft_isdigit(*str))
	{
		str++;
	}
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_isspace(char c)
{
	return (c == ' '
		|| c == '\f'
		|| c == '\n'
		|| c == '\r'
		|| c == '\t'
		|| c == '\v');
}

int	overflow_check(char *result)
{
	char	*end;
	char	*num;

	while (isspace((unsigned char)*result))
		result++;
	end = result;
	if (*result == '+' || *result == '-')
		end++;
	while (*result == '0')
		result++;
	while (ft_isdigit((unsigned char)*end))
		end++;
	while (*end != '\0')
	{
		if (!isspace((unsigned char)*end))
			return (0);
		end++;
	}
	num = ft_strtok(result, " ");
	if (ft_atoi(num) == INT_MAX && (ft_strcmp(num, "2147483647") != 0
			|| ft_strcmp(num, "9223372036854775807") != 0))
		return (1);
	return (0);
}

int	exec_exit(t_exec_vars *vars, t_free_data *exec_data)
{
	int		i;
	char	*result;

	i = 0;
	result = NULL;
	g_last_exit_status = 0;
	if (vars->args[1] != NULL)
	{
		result = vars->args[1];
		if (result[i] == '+' || result[i] == '-')
			i++;
		while (result[i] == '0')
			i++;
		if (handle_exit(result, vars))
			return (g_last_exit_status);
		g_last_exit_status = ft_atoi(result);
	}
	free_exit_data(exec_data);
	clear_history();
	free_env_array(vars->args);
	free(vars);
	exit(g_last_exit_status);
}
