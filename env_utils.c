#include "lexer.h"

t_env	*create_env_var(const char *name, const char *value)
{
	t_env	*var;

	var = MY_MALLOC(sizeof(t_env));
	if (var == NULL)
		return (NULL);
	var->name = ft_strdup(name);
	if (var->name == NULL)
		return (free(var), NULL);
	var->value = ft_strdup(value);
	if (var->value == NULL)
		return (free(var->name), free(var), NULL);
	var->next = NULL;
	return (var);
}

char	*get_env_var(t_env *head, const char *name)
{
	while (head != NULL)
	{
		if (ft_strcmp(head->name, name) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

int	count_env_list(t_env *head)
{
	t_env	*current;
	int		count;

	count = 0;
	current = head;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

t_env	*find_env_var(t_env *head, const char *name)
{
	while (head)
	{
		if (ft_strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

char	*create_env_str(t_env *current)
{
	char	*env_str;

	env_str = MY_MALLOC(ft_strlen(current->name) + ft_strlen(current->value) + 2);
	if (env_str == NULL)
		return (NULL);
	ft_strcpy(env_str, current->name);
	ft_strcat(env_str, "=");
	ft_strcat(env_str, current->value);
	return (env_str);
}
