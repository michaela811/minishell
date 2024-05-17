/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:38:19 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 13:45:02 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_hex_len(unsigned int num)
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

void	ft_put_hex_fd(unsigned int fd, unsigned int num, char placeholder)
{
	if (num >= 16)
	{
		ft_put_hex_fd(fd, num / 16, placeholder);
		ft_put_hex_fd(fd, num % 16, placeholder);
	}
	else
	{
		if (num <= 9)
			ft_print_char_fd(fd, num + '0');
		else
		{
			if (placeholder == 'x')
				ft_print_char_fd(fd, num - 10 + 'a');
			else if (placeholder == 'X')
				ft_print_char_fd(fd, num - 10 + 'A');
		}
	}
}

int	ft_print_hex_fd(unsigned int fd, unsigned int num, char placeholder)
{
	int	return_len;

	return_len = 0;
	if (num == 0)
		return_len += write(fd, "0", 1);// or just rite(fd, "0", 1)
	else
	{
		return_len += ft_hex_len(num);
		ft_put_hex_fd(fd, num, placeholder);
	}
	return (return_len);
}
