/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:09:56 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:25:44 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	char	*str_start;

	str = malloc(sizeof(char) * (ft_strlen((char *)s) + 1));
	if (str == NULL)
		return (NULL);
	str_start = str;
	while (*s)
	{
		*str = *s;
		s++;
		str++;
	}
	*str = '\0';
	return (str_start);
}
/*
int main(void)
{
	char a[] = "Copy me!";
	printf("%s\n", ft_strdup(a));
}
*/
