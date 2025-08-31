/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 20:57:35 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 12:35:10 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c > 64 && c < 91)
		c = c + 32;
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
    printf("%d\n", ft_tolower(input[0]));
    printf("original:%d", tolower(input[0]));
}

int main(void)
{
	char c = '!';
	printf("%c\n", ft_tolower(c));
    printf("original:%c", tolower(c));
}
*/
