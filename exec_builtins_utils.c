#include "minishell.h"

int	var_control(char *command, char *args)
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

static int	is_equals(char **name, char **value, char *equals)
{
	if (equals == NULL)
	{
		*value = ft_strdup("");
		if (value == NULL)
			return (print_err(1, 2,
					"split_var: strndup error\n"), free(*name),
				g_last_exit_status);
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
	if (equals)
		*name = ft_strndup(var, equals - var);
	else
		*name = ft_strdup(var);
	if (*name == NULL)
		return (print_err(1, 2,
				"split_var: strndup error\n"), free(*name), g_last_exit_status);
	if (control_name(var, name, &i))
		return (g_last_exit_status);
	(*name)[i] = '\0';
	if (is_equals(name, value, equals))
		return (g_last_exit_status);
	*value = ft_strdup(equals + 1);
	if (*value == NULL)
		return (print_err(1, 2,
				"split_var: strndup error\n"), free(*name), g_last_exit_status);
	return (0);
}
