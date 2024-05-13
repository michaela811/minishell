/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 09:27:54 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 13:53:28 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_format_fd(unsigned int fd, char placeholder, va_list args)
{
	int	print_len;

	print_len = 0;
	if (placeholder == 'c')
		print_len += ft_print_char_fd(fd, va_arg(args, unsigned int));
	else if (placeholder == 's')
		print_len += ft_print_str_fd(fd, va_arg(args, const char *));
	else if (placeholder == 'p')
		print_len += ft_print_ptr_fd(fd, va_arg(args, unsigned long long));
	else if (placeholder == 'i' || placeholder == 'd')
		print_len += ft_print_int_fd(fd, va_arg(args, int));
	else if (placeholder == 'u')
		print_len += ft_print_unsign_int_fd(fd, va_arg(args, unsigned int));
	else if (placeholder == 'x' || placeholder == 'X')
		print_len += ft_print_hex_fd(fd, va_arg(args, unsigned int), placeholder);
	else if (placeholder == '%')
		print_len += ft_print_percent_fd(fd);
	else
	{
		print_len += ft_print_char_fd(fd, '%');
		print_len += ft_print_char_fd(fd, placeholder);
	}
	return (print_len);
}

int	ft_printf_fd(unsigned int fd, const char *print, ...)
{
	int		return_len;
	va_list	args;
	int		i;

	if (!print)
		return (-1);
	return_len = 0;
	va_start(args, print);
	i = 0;
	while (print[i])
	{
		if (print[i] == '%')
		{
			return_len += ft_format_fd(fd, print[i + 1], args);
			i++;
		}
		else
		{
			ft_print_char_fd(fd, print[i]);
			return_len++;
		}
		i++;
	}
	va_end(args);
	return (return_len);
}
