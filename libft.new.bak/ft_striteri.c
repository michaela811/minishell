/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 14:46:06 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:10:54 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (s == NULL || f == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &(s[i]));
		i++;
	}
}
/*
#include <stdio.h>

void	test_function(unsigned int n, char *a)
{
	if (a != NULL)
	*a = *a + 33;
}

int main()
{
    char input[] = "HKEERGHGERLGJERGEL";
    ft_striteri(input, test_function);
    if (input != NULL)
        printf("%s\n", input);
}
*/
