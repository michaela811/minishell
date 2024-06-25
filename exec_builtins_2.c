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
	int		i;

	i = 0;
	if (args[1] == NULL)
		return (0);
	current = *env;
	prev = NULL;
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
	g_last_exit_status = 0;
	return (0);
}
