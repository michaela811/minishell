/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 18:01:58 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 17:12:00 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <strings.h>
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	while (n)
	{
		*(char *)s = '\0';
		s++;
		n--;
	}
}
/*
#include <stdio.h>
int main(void)
{
    char str[] = "occupied memory";
    ft_bzero(str, 5);
	char *str4 = &str[4];
	printf("modifed memory: %s\n", str4);
	char *str5 = &str[5];
    printf("modifed memory: %s\n", str5);
}
*/
