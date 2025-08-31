/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:28:35 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 17:16:47 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <string.h>
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n != 0)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		s++;
		n--;
	}
	return (0);
}
/*
#include <stdio.h>
int main(void)
{
	char b[] = "abcdeffger";
	char a = 'd';
	int n = 3;

	printf("%s\n", (char *)ft_memchr(b, a, n));
}
*/
