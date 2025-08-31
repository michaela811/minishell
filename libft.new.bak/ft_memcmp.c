/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:01:23 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:09:54 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <string.h>
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	while (n != 0)
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		else
		{
			s1++;
			s2++;
			n--;
		}
	}
	return (0);
}
/*
#include <stdio.h>
int main(void)
{
    char a[] = "abcdeffger";
    char b[] = "abcdeyftgf";
    int n = 2;

    printf("%d\n", ft_memcmp(b, a, n));
	printf("%d\n", memcmp(b, a, n));
}
*/
