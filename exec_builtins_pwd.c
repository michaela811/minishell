/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 11:21:49 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pwd(void)//(t_env **env)
{
	char	*cwd;
	/*char	*pwd_env;

	pwd_env = get_env_var(*env, "PWD");
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		g_last_exit_status = 0;
		return (g_last_exit_status);
	}
	else */
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (print_err(1, 2, "current working directory not set\n"),
			g_last_exit_status);
	else
	{
		printf("%s\n", cwd);
		free(cwd);
		g_last_exit_status = 0;
		return (g_last_exit_status);
	}
}
