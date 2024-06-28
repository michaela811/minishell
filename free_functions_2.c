#include <minishell.h>

void	free_handle_vars(t_handle_vars *l_vars)
{
	if (l_vars->result == NULL)
		return ;
	if (l_vars->result != NULL)
	{
		if (*l_vars->result != NULL)
		{
			free(*l_vars->result);
			*l_vars->result = NULL;
		}
		free(l_vars->result);
		l_vars->result = NULL;
	}
}

void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i] != NULL)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
	env_array = NULL;
}
