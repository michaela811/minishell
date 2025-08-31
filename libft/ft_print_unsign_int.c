/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsign_int.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:03:41 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 14:06:44 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_unsign_int_len(unsigned int num)
{
	int	size;

	size = 0;
	while (num)
	{
		size++;
		num = num / 10;
	}
	return (size);
}

char	*ft_put_unsign_int(unsigned int num)
{
	char	*nbr;
	int		len;

	len = ft_unsign_int_len(num);
	nbr = malloc(sizeof(char) * (len + 1));
	if (!nbr)
		return (0);
	nbr[len] = '\0';
	while (num != 0)
	{
		nbr[len - 1] = num % 10 + '0';
		num = num / 10;
		len--;
	}
	return (nbr);
}

int	ft_print_unsign_int(unsigned int num)
{
	int		return_len;
	char	*nbr;

	return_len = 0;
	if (num == 0)
		return_len += write(1, "0", 1);
	else
	{
		nbr = ft_put_unsign_int(num);
		return_len += ft_print_str(nbr);
		free(nbr);
	}
	return (return_len);
}
