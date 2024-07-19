/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:33:43 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_underscore(t_exec_vars *vars, t_free_data *exec_data)
{
	if (ft_strcmp(vars->args[0], "exit") == 0
			|| ft_strcmp(vars->args[0], "cd") == 0
			|| ft_strcmp(vars->args[0], "pwd") == 0
			|| ft_strcmp(vars->args[0], "export") == 0
			|| ft_strcmp(vars->args[0], "unset") == 0
			|| ft_strcmp(vars->args[0], "env") == 0
			|| (ft_strcmp(vars->args[0], "echo") == 0 && !vars->args[1]))
	{
		if (update_add_env_var(&exec_data->env, "_", vars->args[0]))//maybe different error handling
				return (g_last_exit_status);
	}
	else if (ft_strcmp(vars->args[0], "echo") == 0)
	{
		if (update_add_env_var(&exec_data->env, "_", vars->args[1]))//maybe different error handling
				return (g_last_exit_status);
	}
	return(0);
}

int	exec_builtins(t_exec_vars *vars, t_free_data *exec_data)
{
	if (update_underscore(vars, exec_data))
		return(g_last_exit_status);
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, exec_data));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "pwd") == 0)
		return (exec_pwd(&exec_data->env));
	else if (ft_strcmp(vars->args[0], "echo") == 0)
		return (exec_echo(vars));
	else if (ft_strcmp(vars->args[0], "export") == 0)
		return (exec_export(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "unset") == 0)
		return (exec_unset(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "env") == 0)
		return (exec_env(vars->args, exec_data->environment));
	return (3);
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

int	ft_isspace(char c)
{
	return (c == ' '
		|| c == '\f'
		|| c == '\n'
		|| c == '\r'
		|| c == '\t'
		|| c == '\v');
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

void	return_exit_status(char *result, int *i, int *j, int *error)
{
	char	*temp;

	temp = ft_itoa(g_last_exit_status);
	if (temp == NULL)
	{
		*error = 1;
		print_err(1, 2, "echo: memory allocation\n");
		return ;
	}
	result[*j] = temp[0];
	*j = *j + ft_strlen(temp);
	*i += 2;
	free(temp);
	return ;
}
