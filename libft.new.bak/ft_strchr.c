/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:16:00 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/20 11:21:27 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	i;

	i = (unsigned char) c;
	while (*s != '\0')
	{
		if (*s == i)
			return ((char *)s);
		s++;
	}
	if (!i && *s == '\0')
		return ((char *)s);
	return (0);
}
/*
#include <string.h>
#include <stdio.h>
int main(void)
{
	char *str = "teste";
	printf("my funcion:%s\n",ft_strchr(str, 't' + 256));
	printf("%p\n", ft_strchr(str, '\0'));
	printf("original function:%s\n",strchr(str, 't' + 256));
	printf("%p\n", strchr(str, '\0'));
}
*/
