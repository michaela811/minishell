/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:01:47 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 17:12:58 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58))
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
    printf ("%d", ft_isalnum(*argv[1]));
}
*/
