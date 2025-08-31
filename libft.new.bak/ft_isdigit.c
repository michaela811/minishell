/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:48:52 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/14 16:51:51 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c > 47 && c < 58)
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
    printf ("%d", ft_isdigit(*argv[1]));
}
*/
