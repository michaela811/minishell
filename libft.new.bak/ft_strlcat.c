/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 15:44:55 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/17 21:04:26 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, unsigned int size)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	while (dest[i] && i < size)
		i++;
	while (src[j] && ((i + j + 1) < size))
	{
		dest[i + j] = src[j];
		j++;
	}
	if (i < size)
		dest[i + j] = '\0';
	return (i + ft_strlen(src));
}
/* 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
	char *a = "abcde";
	char *b = "efgge";
	int n = 15;
	printf("%d\n", ft_strlcat(a, b, n));
	printf("%s", a);
}
int main(int argc, char **argv)
{
	//unsigned int length;
	//char a[atoi(argv[1])];
    if (argc != 4)
        return (0);
    printf("%d\n", ft_strlcat(argv[1], argv[2], atoi(argv[3])));
    printf("\n%s\n", argv[1]);
    //printf("%d\n", length);

}*/
