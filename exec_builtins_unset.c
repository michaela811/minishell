/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 14:55:50 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (0);
}
