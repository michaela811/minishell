/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:49 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:33:51 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_env(char **args, char **environment)
{
	int	i;

	i = 0;
	if (args[1] != NULL)
		return(print_err(1, 2, "env: too many arguments\n"), 1);
	while (environment[i] != NULL)
		printf("%s\n", environment[i++]);
	g_last_exit_status = 0;
	return (g_last_exit_status);
}