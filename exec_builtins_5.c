/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:34:12 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 12:15:53 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	exec_exit(t_exec_vars *vars, t_free_data *exec_data)
{
	int		i;
	char	*result;

	i = 0;
	result = NULL;
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

int	echo_n_control(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] == '-' && arg[i] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (1);
	}
	return (0);
}

int	echo_n_control2(char *arg)
{
	int	i;

	i = 0;
	while (arg && arg[i])
	{
		while (arg[i] == ' ')
			i++;
		if (arg[i] == '-' && arg[i + 1] == 'n')
		{
			i += 2;
			while (arg[i] == 'n')
				i++;
			if (arg[i] == '\0' || arg[i] == ' ')
				continue;
		}
		return (i);
	}
	return (i);
}

int echo_no_n(char **args, int j)
{
	char *new_arg;

    if (j)
    {
        new_arg = ft_strdup(&(*args)[j]);
        if (!new_arg)
            return(print_err(1, 2, "malloc error in echo\n"), 1);
        free(*args);
        *args = new_arg;
    }
    return 0;
}

int	exec_echo(t_exec_vars *vars)
{
	int	i;
	int j;

	i = 1;
	if (vars->args[1] == NULL)
		return (print_err(g_last_exit_status, vars->fd_out, "\n"), 1);
	//while (vars->args[i] && echo_n_control(vars->args[i]))
		//i++;
	j = echo_n_control2(vars->args[1]);
	if (echo_no_n(&vars->args[1], j))
		return (print_err(1, 2,
				"malloc error in echo\n"), 1);
	while (vars->args[i])
	{
		print_err(g_last_exit_status, vars->fd_out, "%s",
			vars->args[i]);
		if (vars->args[i + 1])
			print_err(g_last_exit_status, vars->fd_out, " ");
		i++;
	}
	//if (echo_n_control(vars->args[1]) == 0)
	if (j == 0)
		print_err(g_last_exit_status, vars->fd_out, "\n");
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

void	process_args(char **args, int *error)
{
	int		i;
	char	*new_str;

	i = 1;
	while (args[i])
	{
		new_str = handle_quotes_echo((args[i]), error);
		if (*error)
		{
			free(new_str);
			return ;
		}
		if (args[i])
			free(args[i]);
		args[i] = new_str;
		i++;
	}
}
