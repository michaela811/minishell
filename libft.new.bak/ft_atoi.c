/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 15:08:11 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 17:11:40 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	count;
	int	number;

	i = 0;
	count = 0;
	number = 0;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			count = 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		number = number * 10 + (nptr[i] - '0');
		i++;
	}
	if ((count % 2) == 1)
		return (-1 * number);
	return (number);
}
/*
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	if (argc != 2)
		return (0);	
	printf("%d\n", ft_atoi(argv[1]));
	printf("%d\n", atoi(argv[1]));
}
*/
