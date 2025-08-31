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

#include "ft_printf.h"

int	ft_format(char placeholder, va_list args)
{
	int	print_len;

	print_len = 0;
	if (placeholder == 'c')
		print_len += ft_print_char(va_arg(args, unsigned int));
	else if (placeholder == 's')
		print_len += ft_print_str(va_arg(args, const char *));
	else if (placeholder == 'p')
		print_len += ft_print_ptr(va_arg(args, unsigned long long));
	else if (placeholder == 'i' || placeholder == 'd')
		print_len += ft_print_int(va_arg(args, int));
	else if (placeholder == 'u')
		print_len += ft_print_unsign_int(va_arg(args, unsigned int));
	else if (placeholder == 'x' || placeholder == 'X')
		print_len += ft_print_hex(va_arg(args, unsigned int), placeholder);
	else if (placeholder == '%')
		print_len += ft_print_percent();
	else
	{
		print_len += ft_print_char('%');
		print_len += ft_print_char(placeholder);
	}
	return (print_len);
}

int	ft_printf(const char *print, ...)
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
			return_len += ft_format(print[i + 1], args);
			i++;
		}
		else
		{
			ft_print_char(print[i]);
			return_len++;
		}
		i++;
	}
	va_end(args);
	return (return_len);
}
