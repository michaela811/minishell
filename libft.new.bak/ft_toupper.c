/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 20:22:16 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 12:35:52 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c > 96 && c < 123)
		c = c - 32;
	return (c);
}
/*
#include <ctype.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	if (argc != 2)
		return 0;
	char *input = argv[1];
	printf("%d\n", ft_toupper(input[0]));
	printf("original:%d", toupper(input[0])); 
}
*/
