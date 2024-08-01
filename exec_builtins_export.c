/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:41 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 13:12:08 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_export(char **args, t_env **env)
{
	char	*name;
	char	*value;
	int		control;
	int		empty;
	int		i;

	i = 1;
	empty = 0;
	init_free_name_value(&name, &value, i);
	if (args[1] == NULL)
		return (exec_export_no_args(*env), 0);
	while (args[i] != NULL)
	{
		control = var_control(args[0], args[1], &empty);
		if (control == 1)
			return (g_last_exit_status);
		if (empty == 1)
			break;
		else
		{
			if (split_to_name_value(args, &name, &value, &i))
				continue ;
			if (exec_update_add_env_var(env, name, value))
				continue ;
			init_free_name_value(&name, &value, ++i);
		}
	}
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

void	init_free_name_value(char **name, char **value, int i)
{
	if (i == 1)
	{
		*name = NULL;
		*value = NULL;
	}
	else
	{
		if (name != NULL)
			free(*name);
		if (value != NULL)
			free(*value);
	}
}

void	exec_export_no_args(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		printf("export %s=\"%s\"\n", current->name, current->value);
		current = current->next;
	}
}

int	var_control(char *command, char *args, int *empty)
{
	int	i;

	i = 0;
	if (args[i] == '=' || args[i] == 0 || (!ft_isalpha(args[i])
			&& args[i] != '_'))
	{
		return (print_err(1, 2,
				"mini(s)hell: %s: `%s': not a valid identifier\n",
				command, args), g_last_exit_status);
	}
	i++;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
		{
			return (print_err(1, 2,
					"mini(s)hell: %s: `%s': not a valid identifier\n",
					command, args), g_last_exit_status);
		}
		i++;
	}
	if (ft_strchr(args, '=') == NULL)
		*empty = 1;
	return (0);
}
