/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_for_exec_colon.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 17:16:50 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	join_and_free(char **pre_path, const char *str, int is_prefix)
{
	char	*tmp_path;

	if (is_prefix)
		tmp_path = ft_strjoin(str, *pre_path);
	else
		tmp_path = ft_strjoin(*pre_path, str);
	if (!tmp_path)
		return (free(*pre_path), 1);
	free(*pre_path);
	*pre_path = tmp_path;
	return (0);
}

int	count_double_colon(char *pre_path)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (pre_path[i])
	{
		if (pre_path[i] == ':' && pre_path[i + 1] == ':')
			count++;
		i++;
	}
	return (count);
}

int	process_path_segment(const char *original, char *new_path, int i)
{
	int	j;

	j = 0;
	while (original[i])
	{
		if (original[i] == ':' && original[i + 1] == ':')
		{
			new_path[j++] = ':';
			new_path[j++] = '.';
			i++;
		}
		else
			new_path[j++] = original[i++];
	}
	return (j);
}

int	detect_double_colons(char **pre_path)
{
	char	*original;
	char	*new_path;
	int		i;
	int		j;

	i = 0;
	original = *pre_path;
	new_path = malloc(ft_strlen(original) + count_double_colon(original) + 1);
	if (!new_path)
		return (print_err(1, 2, "my(s)hell: malloc error 11\n"), 1);
	j = process_path_segment(original, new_path, i);
	new_path[j] = '\0';
	free(*pre_path);
	*pre_path = new_path;
	return (0);
}

int	handle_colon(char **pre_path, t_env *env)
{
	int		len;
	char	*tmp_path;

	tmp_path = get_env_var(env, "PATH");
	if (!tmp_path)
		return (0);
	*pre_path = ft_strdup(tmp_path);
	if (!*pre_path)
		return (print_err(1, 2, "my(s)hell: malloc error 12\n"), 1);
	if ((*pre_path)[0] == ':')
	{
		if (join_and_free(pre_path, ".", 1))
			return (1);
	}
	len = ft_strlen(*pre_path);
	if (len > 0 && (*pre_path)[len - 1] == ':')
	{
		if (join_and_free(pre_path, ".", 0))
			return (1);
	}
	if (detect_double_colons(pre_path))
		return (free(*pre_path), 1);
	return (0);
}
