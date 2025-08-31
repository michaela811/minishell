/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 20:14:33 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/17 21:09:44 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, unsigned int size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && (i < size - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest [i] = '\0';
	return (ft_strlen(src));
}
/*
int	main(int argc, char *argv[])
{
	//unsigned int length;
	//char a[atoi(argv[1])];
	if (argc != 4)
		return (0);
	printf("%d", ft_strlcpy(argv[1], argv[2], atoi(argv[3])));
	//printf("\n%s\n", argv[1]);
	//print("%d", length);
  }
*/
/*
int main(void)
{
    char string[] = "abcdef";
    char pointed_buffer[10];
	unsigned int size = 10;
	unsigned int length;

	length = ft_strlcpy(pointed_buffer, string, 10);
    printf("%d", length);
}
*/
