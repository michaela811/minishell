/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:18:30 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/20 11:24:31 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
//#include <string.h>

#include "libft.h"

static char	**error_check(char **array, int index);

//Control strncpy!
char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	int	i;

	i = 0;
	while (src[i] && n)
	{
		dst[i] = src[i];
		i++;
		n--;
	}
	while (n > 0)
	{
		dst[i] = '\0';
		i++;
		n--;
	}
	return (dst);
}

static int	count(char const *s, char c)
{
	long unsigned int	i;
	int					count;
	int					word;

	i = 0;
	word = 0;
	count = 0;
	while (i < ft_strlen((char *)s))
	{
		if (s[i] != c)
		{
			if (word == 0)
			{
				count++;
				word = 1;
			}
		}
		else
			word = 0;
		i++;
	}
	return (count);
}

static char	**assign_values(char **start, int *n, int *str_index, char const *s)
{
	char	**strs;

	*start = (char *)s;
	*str_index = 0;
	strs = malloc (sizeof(char *) * (*n + 1));
	if (!strs)
		return (NULL);
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	int		n;
	char	**strs;
	char	*start;
	int		str_index;

	n = count(s, c);
	strs = assign_values(&start, &n, &str_index, s);
	if (!strs)
		return (NULL);
	while (str_index != n)
	{
		if (*s == c || *s == '\0')
		{
			if (s != start)
			{
				strs[str_index] = malloc (s - start + 1);
				error_check(strs, str_index);
				ft_strncpy (strs[str_index], start, (s - start));
				strs[str_index++][s - start] = '\0';
			}
			start = (char *)(s + 1);
		}
		s++;
	}
	return (strs[str_index] = NULL, strs);
}

static char	**error_check(char **array, int index)
{
	if (!array[index])
	{
		index--;
		while (index >= 0)
			free(array[index--]);
		free (array);
		return (NULL);
	}
	return (array);
}
/*
int main(void)
{
    char *input = ",Alpaca,,.the animal,. who. rules. the world!,,..  final.,";
    char a = ',';
    char **output = ft_split(input, a);
    
    int rows = 0;
    while (output[rows] != NULL)
    {
        rows++;
    }
    
    for (int k = 0; k < rows; k++)
    {
        for (int l = 0; output[k][l] != '\0'; l++)
        {
            printf("%c", output[k][l]);
        }
        printf("\n");
    }

    for (int k = 0; k < rows; k++)
    {
        free(output[k]);
    }
    free(output);

    return(0);
}
*/
