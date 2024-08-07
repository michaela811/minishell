/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:30:37 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 15:16:11 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_environment(char **envt)
{
	t_env	*head;
	char	**env;
	char	*entry;
	char	*separator;
	t_env	*var;

	head = NULL;
	env = envt;
	while (*env)
	{
		entry = *env;
		separator = ft_strchr(entry, '=');
		if (separator)
		{
			*separator = '\0';
			var = create_env_var(entry, separator + 1);
			if (var == NULL)
				return (free_env(head), NULL);
			*separator = '=';
			var->next = head;
			head = var;
		}
		env++;
	}
	return (head);
}

void	reverse_free_env_array(char **env_array, int count)
{
	while (env_array[count] != NULL)
	{
		free(env_array[count]);
		env_array[count] = NULL;
		count--;
	}
	free(env_array);
	env_array = NULL;
}

char	**env_list_to_array(t_env *head)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;

	count = count_env_list(head);
	env_array = malloc((count + 1) * sizeof(char *));
	if (env_array == NULL)
		return (NULL);
	current = head;
	i = count - 1;
	while (current != NULL)
	{
		env_array[i] = create_env_str(current);
		if (env_array[i] == NULL)
		{
			reverse_free_env_array(env_array, i);
			return (NULL);
		}
		current = current->next;
		i--;
	}
	env_array[count] = NULL;
	return (env_array);
}

int	get_exec(char **path_array, int i, char *cmd, char **path)
{
	char	*path_part;

	path_part = ft_strjoin(path_array[i], "/");
	if (path_part == NULL)
		return (print_err(1, 2,
				"my(s)hell: malloc error in strjoin function\n"), 1);
	*path = ft_strjoin(path_part, cmd);
	if (*path == NULL)
	{
		free(path_part);
		return (print_err(1, 2,
				"my(s)hell: malloc error in strjoin function\n"), 1);
	}
	free(path_part);
	return (0);
}
