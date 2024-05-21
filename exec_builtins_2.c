#include "minishell.h"

int	exec_env(char **args, char **environment)
{
	int	i;

	i = 0;
	if (args[1] != NULL)
		return (perror("env: too many arguments\n"), 1);
	while (environment[i] != NULL)
		printf("%s\n", environment[i++]);
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

int	exec_unset(char **args, t_env **env)
{
	t_env	*current;
	t_env	*prev;

	if (args[1] == NULL)
		return (0);
	current = *env;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, args[1]) == 0)
		{
			if (prev == NULL)
				*env = current->next;
			else
				prev->next = current->next;
			MY_FREE(current->name);
			MY_FREE(current->value);
			MY_FREE(current);
			g_last_exit_status = 0;
			return (g_last_exit_status);
		}
		prev = current;
		current = current->next;
	}
	g_last_exit_status = 0;
	return (0);//IF NOT FOUND, IT IS NOT AN ERROR?
}
