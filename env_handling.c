/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:30:37 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 14:30:00 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_add_env_var(t_env **head, const char *name, const char *value)
{
	t_env	*var;
	t_env	*new_var;

	var = find_env_var(*head, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
		if (var->value == NULL)
			return (print_err(ENOMEM, 2,
					"my(s)hell: malloc error in strdup function\n"), 1);
	}
	else
	{
		new_var = create_env_var(name, value);
		if (new_var == NULL)
			return (print_err(ENOMEM, 2,
					"my(s)hell: malloc error in strdup function\n"), 1);
		new_var->next = *head;
		*head = new_var;
	}
	return (0);
}
