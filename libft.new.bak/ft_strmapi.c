/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 11:19:47 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:11:14 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	i = 0;
	str = malloc ((ft_strlen((char *)s) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*
#include <stdio.h>

char	test_function(unsigned int n, char a)
{
	if (n % 10 == 0)
		return(a + 33);
	else
		return (a);
}	

int	main()
{
	const char *input = "HKEERGHGERLGJERGEL";
    char *result = ft_strmapi(input, test_function);
    if (result != NULL) 
	{
        printf("%s\n", result);
        free(result);
    }
    return 0;
}
*/
