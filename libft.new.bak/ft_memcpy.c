/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 18:47:56 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:10:08 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <string.h>
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*start;

	start = (unsigned char *)dest;
	if (!dest && !src)
		return (0);
	while (n)
	{
		*(unsigned char *)dest = *(unsigned char *)src;
		n--;
		src++;
		dest++;
	}
	return (start);
}
/*
#include <stdio.h>
#include <stdio.h>
int main(void)
{
    char src[] = "source string";
    char dest[20];
    dest[0] = src[3];
    printf("dest before:%s\n", dest);
    printf("src before:%s\n", src);
    ft_memcpy(dest, src, 5);
    printf("dest string1:%s\n", dest);
    memcpy(dest, src, 5);
    printf("dest string2:%s", dest);
}
*/
/*
int main(void)
{
	char src[] = "source string";
	char dest[20];
	memcpy(dest, src, 5);
	printf("dest string:%s", dest);
}
*/
/*
int main(void)
{
	int a = 2;
	int b = 3;
	memcpy(&a,&b, 1);
	printf("%d", a);
}
*/
