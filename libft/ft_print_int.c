/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:53:47 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 13:56:53 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_int_len(int num)
{
	int	size;

	size = 0;
	if (num < 0)
	{
		num *= -1;
		size += 1;
	}
	if (num == 0)
		size += 1;
	while (num)
	{
		num = num / 10;
		size++;
	}
	return (size);
}

void	ft_put_int(int num)
{
	if (num == -2147483648)
	{
		write(1, "-2", 2);
		num = 147483648;
	}
	if (num < 0)
	{
		ft_print_char('-');
		num *= -1;
	}
	if (num >= 10)
	{
		ft_put_int(num / 10);
		ft_put_int(num % 10);
	}
	if (num <= 9)
		ft_print_char(num + '0');
}

int	ft_print_int(int nbr)
{
	int	return_len;

	return_len = 0;
	return_len += ft_int_len(nbr);
	ft_put_int(nbr);
	return (return_len);
}
