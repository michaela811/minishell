/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:53:57 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 11:28:55 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	num_char;

	num_char = 0;
	while (*str)
	{
		num_char++;
		str++;
	}
	return (num_char);
}
/*
#include <stdio.h>
int main(int argc, char**argv)
{
	if (argc != 2)
		return(0);
	printf("%d", ft_strlen(argv[1]));
}
*/
