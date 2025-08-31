/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 14:15:37 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/20 11:23:46 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"
#include <bsd/string.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (*little == '\0')
		return ((char *)big);
	while (*big && len)
	{
		i = 0;
		while (big[i] == little[i] && little[i] != '\0' && i < len)
			i++;
		if (little[i] == '\0')
			return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}
/*
#include <stdio.h>
int main(void)
{
	const char largestring[30] = "aaabcabcd";
    const char smallstring[10] = "aabc";
    char *ptr1;
	//char *ptr2;

    ptr1 = ft_strnstr(largestring, smallstring, -1);
	//ptr2 = strnstr(largestring, smallstring, 6);
	printf("%s\n", ptr1);
	//printf("%s\n", ptr2);
}
*/
