/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:35:32 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/20 11:26:41 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	i;
	const char		*start;

	i = (unsigned char) c;
	start = s;
	s = (s + ft_strlen(s));
	while (s != start && i != (unsigned char)(*s))
		s--;
	if (i == (unsigned char)(*s))
		return ((char *)s);
	return (NULL);
}
/*
#include <string.h>
#include <stdio.h>
int main(void)
{
    char *str = "pepe y cparlos";
    printf("my funcion:%s\n",ft_strrchr(str, 'c'));
    printf("original function:%s",strrchr(str, 'c'));
}
*/
