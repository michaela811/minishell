/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:23:43 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:10:17 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <strings.h>
#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t n)
{
	unsigned int	i;

	i = n;
	while (n)
	{
		*(char *)ptr = value;
		ptr++;
		n = n - 1;
	}
	return (ptr - i);
}
/*
#include <stdio.h>
int main(void)
{
	char str[] = "occupied memory";
	ft_memset(str, 33, 5);
	printf("modifided memory: %s\n", str);
}
*/
