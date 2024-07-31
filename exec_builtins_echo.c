/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:56 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/18 16:05:45 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	exec_echo(t_exec_vars *vars)
{
	int	i;
	//int j;

	i = 1;
	if (vars->args[1] == NULL)
	{
		g_last_exit_status = 0;
		return (print_echo(g_last_exit_status, vars->fd_out, "\n"), 1);
	}
	while (vars->args[i] && echo_n_control(vars->args[i]))
		i++;
	//j = echo_n_control2(vars->args[1]);
	//if (echo_no_n(&vars->args[1], j))
		//return (print_err(1, 2,
				//"malloc error in echo\n"), 1);
	while (vars->args[i])
	{
		print_echo(g_last_exit_status, vars->fd_out, "%s",
			vars->args[i]);
		if (vars->args[i + 1])
			print_echo(g_last_exit_status, vars->fd_out, " ");
		i++;
	}
	if (echo_n_control(vars->args[1]) == 0)
	//if (j == 0)
		print_echo(g_last_exit_status, vars->fd_out, "\n");
	g_last_exit_status = 0;
	return (g_last_exit_status);
}