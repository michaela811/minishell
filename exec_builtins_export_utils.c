/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_export_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 13:03:57 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_to_name_value(char **args, char **name, char **value, int *i)
{
	if (split_var(args[*i], name, value))
	{
		g_last_exit_status = 1;
		//(*i)++;
		return (1);
	}
	return (0);
}

int	split_var(char *var, char **name, char **value)
{
	char	*equals;
	int		i;

	i = 1;
	equals = ft_strchr(var, '=');
	//if (equals)
	*name = ft_strndup(var, equals - var);
	if (*name == NULL)
		return (print_err(ENOMEM, 2,
				"split_var: strndup error\n"), free(*name), g_last_exit_status);
	if (control_name(var, name, &i))
		return (g_last_exit_status);
	(*name)[i] = '\0';
	if (is_equals(name, value, equals))
		return (g_last_exit_status);
	*value = ft_strdup(equals + 1);
	if (*value == NULL)
		return (print_err(ENOMEM, 2,
				"split_var: strndup error\n"), free(*name), g_last_exit_status);
	return (0);
}

int	control_name(char *var, char **name, int *i)
{
	while ((*name)[*i] != '\0')
	{
		if (!ft_isalnum((*name)[*i]) && (*name)[*i] != '_')
			return (print_err(1, 2,
					"mini(s)hell: %s: `%s': not a valid identifier\n",
					var[0], *name), free(*name), g_last_exit_status);
		(*i)++;
	}
	return (0);
}

int	is_equals(char **name, char **value, char *equals)
{
	if (equals == NULL)
	{
		*value = ft_strdup("");
		if (value == NULL)
			return (print_err(ENOMEM, 2,
					"split_var: strndup error\n"), free(*name),
				g_last_exit_status);
		return (1);
	}
	return (0);
}

int	exec_update_add_env_var(t_env **env, char *name, char *value)
{
	if (update_add_env_var(env, name, value))
	{
		if (name != NULL)
			free(name);
		if (value != NULL)
			free(value);
		g_last_exit_status = 1;
		return (1);
	}
	return (0);
}
