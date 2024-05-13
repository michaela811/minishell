/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:57:10 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 13:58:02 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_print_str_fd(unsigned int fd, const char *str)
{
	int	i;

	i = 0;
	if (str == 0)
		return (write(fd, "(null)", 6));
	while (*str)
	{
		ft_print_char_fd(fd, *str);
		str++;
		i++;
	}
	return (i);
}
