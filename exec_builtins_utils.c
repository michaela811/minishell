#include "lexer.h"

int	change_directory_and_update(char *path, t_env **env, char *cwd)
{
	if (chdir(path) != 0)
	{
		g_last_exit_status = 1;
		perror("chdir");
		free(cwd);
		return (1);
	}
	if (update_pwd(env, cwd))
	{
		g_last_exit_status = 1;
		free(cwd);
		return (1);
	}
	free(cwd);
	return (0);
}

void	exec_export_no_args(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		printf("declare -x %s=\"%s\"\n", current->name, current->value);
		current = current->next;
	}
}

int	var_control(char *args)
{
	int	i;

	i = 0;
	if (args[i++] == '=')
		return (perror("export: not a valid identifier\n"), 1);
	return (0);
	/*while (args[i])
	{
		if (args[i++] == '=')//Seems like in bash it is not a problem not to have =
			return (0);
	}
	return (perror("export: not a valid identifier\n"), 1);*/
}

int	split_var(char *var, char **name, char **value)
{
	char	*equals;
	int		i;

	i = 0;
	equals = ft_strchr(var, '=');
	if (equals)
		*name = ft_strndup(var, equals - var);
	else
		*name = ft_strdup(var);
	if (name == NULL)
		return (perror("split_var: strndup error\n"), 1);
	while ((*name)[i] != '\0')
	{
		if (ft_isalpha((*name)[i]) == 0)
			return (perror("export: not a valid identifier\n"), free(*name), 1);
		i++;
	}
	(*name)[i] = '\0';
	if (equals == NULL)
	{
		*value = ft_strdup("");
		if (value == NULL)
			return (perror("split_var: strndup error\n"), free(*name), 1);
		return (0);
	}
	*value = ft_strdup(equals + 1);
	if (value == NULL)
		return (perror("split_var: strndup error\n"), free(*name), 1);
	return (0);
}

int	update_pwd(t_env **env, char *cwd)
{
	if (update_add_env_var(env, "OLDPWD", cwd))
		return (1);
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (perror("getcwd"), 1);
	if (update_add_env_var(env, "PWD", cwd))
		return (free(cwd), 1);
	return (free(cwd), 0);
}
