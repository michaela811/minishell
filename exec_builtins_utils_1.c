#include <minishell.h>

int	export_quotes(char *input, char **output)
{
	int	len;

	len = ft_strlen(input);
	if (len == 0)
	{
		*output = ft_strdup("");
		return (0);
	}
	if ((input[0] == '"' || input[0] == '\'') && input[0] != input[len - 1])
	{
		g_last_exit_status = 1;
		return (ft_printf_fd(1, "export_quotes: unbalanced quotes"),
			g_last_exit_status);
	}
	if ((input[0] == '"' || input[0] == '\'') && input[0] == input[len - 1])
		*output = ft_strndup(input + 1, len - 2);
	else
		*output = ft_strdup(input);
	if (*output == NULL)
	{
		g_last_exit_status = 1;
		return (ft_printf_fd(1, "export_quotes: strndup error"),
			g_last_exit_status);
	}
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

int	change_directory_and_update(char *path, t_env **env, char *cwd, char **args)
{
	if (chdir(path) != 0)
	{
		print_err(1, 2,
			"my(s)hell: %s: %s: No such file or directory\n", args[0], path);
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
	g_last_exit_status = 0;
	return (g_last_exit_status);
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
