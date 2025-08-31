/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:57:38 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:22:39 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
//#include <stdio.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned long	i;
	unsigned long	memory;
	void			*array;
	char			*char_array;

	i = 0;
	memory = nmemb * size;
	if ((nmemb && size) && memory / nmemb != size)
		return (NULL);
	array = malloc(memory);
	if (array == NULL)
		return (NULL);
	char_array = (char *) array;
	while (i < (nmemb * size))
	{
		char_array[i] = '\0';
		i++;
	}
	return ((void *)array);
}
/*
int main() 
{
    size_t nmemb = 10;
    size_t size = sizeof(int);

    int *ptr = ft_calloc(nmemb, size);

    if (ptr != NULL) 
	{
        for (size_t i = 0; i < nmemb; i++) 
            printf("ptr[%zu] = %d\n", i, ptr[i]);
        free(ptr);
    } 
	else 
        fprintf(stderr, "Memory allocation failed\n");
    return 0;
}
*/
