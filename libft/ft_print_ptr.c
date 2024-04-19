/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:45:41 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 13:48:57 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_ptr_len(unsigned long long num)
{
	int	size;

	size = 0;
	while (num)
	{
		num = num / 16;
		size++;
	}
	return (size);
}

void	ft_put_ptr(unsigned long long ptr)
{
	if (ptr >= 16)
	{
		ft_put_ptr(ptr / 16);
		ft_put_ptr(ptr % 16);
	}
	else
	{
		if (ptr <= 9)
			ft_print_char(ptr + '0');
		else
			ft_print_char(ptr - 10 + 'a');
	}
}

int	ft_print_ptr(unsigned long long ptr)
{
	int	return_len;

	return_len = 0;
	if (ptr == 0)
		return_len = write(1, "(nil)", 5);
	else
	{
		return_len += write(1, "0x", 2);
		return_len += ft_ptr_len(ptr);
		ft_put_ptr(ptr);
	}
	return (return_len);
}
