#include <minishell.h>

int	exec_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (print_err(1, 2, "getcwd\n"), g_last_exit_status);
	else
	{
		printf("%s\n", cwd);
		free(cwd);
		g_last_exit_status = 0;
		return (g_last_exit_status);
	}
}

int	split_to_name_value(char **args, char **name, char **value, int *i)
{
	if (split_var(args[*i], name, value))
	{
		g_last_exit_status = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

int	exec_update_add_env_var(t_env **env, char *name, char *value)
{
	if (update_add_env_var(env, name, value))
	{
		free(name);
		free(value);
		g_last_exit_status = 1;
		return (1);
	}
	return (0);
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
		free(*name);
		free(*value);
	}
}

int	exec_export(char **args, t_env **env)
{
	char	*name;
	char	*value;
	int		control;
	int		i;

	i = 1;
	init_free_name_value(&name, &value, i);
	if (args[1] == NULL)
		return (exec_export_no_args(*env), 0);
	while (args[i] != NULL)
	{
		control = var_control(args[0], args[1]);
		if (control == 1)
			return (g_last_exit_status);
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
