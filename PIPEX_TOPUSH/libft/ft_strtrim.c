/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:48:00 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/13 15:48:58 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_char_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	int		start;
	int		end;
	int		i;

	i = 0;
	start = 0;
	while (s1[start] && ft_char_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_char_set(s1[end - 1], set))
		end--;
	new_str = malloc(sizeof(char) * (end - start + 1));
	if (!new_str)
		return (NULL);
	while (start < end)
	{
		new_str[i] = s1[start];
		start++;
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
