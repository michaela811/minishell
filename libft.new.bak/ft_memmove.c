/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:09:49 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/17 19:32:35 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <string.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	unsigned char		*start;

	start = (unsigned char *)dest;
	d = dest;
	s = src;
	if (!dest && !src)
		return (NULL);
	if (dest < src)
	{
		while (n--)
		{
			*d = *s;
			d++;
			s++;
		}
	}
	else
	{
		while (n--)
			*(d + n) = *(s + n);
	}
	return (start);
}
/*
#include <stdio.h>
int main(void)
{
    char src[] = "source string";
    char dest[20];
	dest[0] = src[3];
	char src1[] = "source string";
    char dest1[20];
    dest1[0] = src1[3];
	printf("dest before:%s\n", dest);
    printf("src before:%s\n", src);
    ft_memmove(dest, src, 5);
    printf("dest string1:%d\n", *dest);
	memmove(dest1, src1, 5);
    printf("dest string2:%d", *dest);
}
*/
