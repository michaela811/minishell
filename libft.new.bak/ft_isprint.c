/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:36:42 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/14 16:56:02 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if (c > 31 && c < 127)
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
    printf ("%d", ft_isprint(*argv[1]));
}
*/
