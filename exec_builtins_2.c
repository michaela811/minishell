#include "lexer.h"

int	exec_env(char **args, t_env **env, char **environment)
{
	int	i;

	i = 0;
	if (args[1] != NULL)
		return (perror("env: too many arguments\n"), 1);
	while (environment[i] != NULL)
		printf("%s\n", environment[i++]);
	return (0);
}

int	exec_unset(char **args, t_env **env)
{
	t_env	*current;
	t_env	*prev;

	if (args[2] != NULL)
		return (perror ("unset: too many arguments\n"), 1);
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
			free(current->name);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);//IF NOT FOUND, IT IS NOT AN ERROR?
}
