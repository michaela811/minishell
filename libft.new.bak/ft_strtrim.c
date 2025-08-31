/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:20:02 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:37:29 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//#include <stdlib.h>
#include "libft.h"

static int	ft_strtrim_start(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	start = 1;
	if (*set == '\0')
		return (0);
	while (start)
	{
		j = 0;
		while (set[j] != '\0')
		{
			start = 0;
			if (s1[i] == set[j])
			{
				i++;
				start = 1;
				break ;
			}
			j++;
		}
	}
	return (i);
}

static int	ft_strtrim_end(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	end;

	i = ft_strlen((char *)s1) - 1;
	end = 1;
	if (*set == '\0')
		return (i);
	while (end)
	{
		j = 0;
		while (set[j] != '\0')
		{
			end = 0;
			if (s1[i] == set[j])
			{
				i--;
				end = 1;
				break ;
			}
			j++;
		}
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	len;

	start = ft_strtrim_start(s1, set);
	len = ft_strtrim_end (s1, set) - start + 1;
	return (ft_substr (s1, start, len));
}

/*
int main (void)
{
	char a[] = "  ,,Copy me, again and again!,,  ";
	char b[] = " ,";
	char *trimmed = ft_strtrim(a, b);
    if (trimmed != NULL)
	{
		printf("%s\n", trimmed);
		free(trimmed);
	}
}
*/
