/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:49:53 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:00:13 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (0 <= c && c < 128)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc != 2)
        return(0);
    printf ("%d", ft_isascii(*argv[1]));
}
*/
/*
int main()
{
	printf ("%d", ft_isascii(-1));
}
*/
