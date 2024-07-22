/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_2.c                                  :+:      :+:    :+:   */
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

static void	handle_unset(t_env *current, t_env *prev, t_env **env,
						char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		current = *env;
		prev = NULL;
		while (current != NULL)
		{
			if (ft_strcmp(current->name, args[i]) == 0)
			{
				if (prev == NULL)
					*env = current->next;
				else
					prev->next = current->next;
				free(current->name);
				free(current->value);
				free(current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
}

int	exec_unset(char **args, t_env **env)
{
	t_env	*current;
	t_env	*prev;

	if (args[1] == NULL)
		return (0);
	current = *env;
	prev = NULL;

	handle_unset(current, prev, env, args);
	g_last_exit_status = 0;
	return (0);
}
