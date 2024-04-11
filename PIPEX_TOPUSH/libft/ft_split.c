/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:40:04 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/13 15:26:39 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **array, int j)
{
	while (--j >= 0)
		free(array[j]);
	free(array);
}

static size_t	n_words(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] != c)
		{
			count++;
			while (str[i] != c && str[i])
				i++;
		}
		else
			i++;
	}
	return (count);
}

static size_t	size_word(const char *s, char c, int i)
{
	size_t	size;

	size = 0;
	while (s[i] != c && s[i])
	{
		size++;
		i++;
	}
	return (size);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	i;
	size_t	j;
	size_t	size;

	i = 0;
	j = -1;
	array = malloc(sizeof(char *) * (n_words(s, c) + 1));
	if (!array)
		return (NULL);
	while (++j < n_words(s, c))
	{
		while (s[i] == c)
			i++;
		size = size_word(s, c, i);
		array[j] = ft_substr(s, i, size);
		if (!array[j])
		{
			ft_free(array, j);
			return (NULL);
		}
		i = i + size;
	}
	array[j] = 0;
	return (array);
}
