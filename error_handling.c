/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:30 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 11:24:44 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_err(int status, unsigned int fd, char *format, ...)
{
	va_list		args;
	va_list		args_copy;

	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	ft_vprintf_fd(fd, format, args_copy);
	va_end(args_copy);
	va_end(args);
}

void	print_echo(int status, unsigned int fd, char *format, ...)
{
	va_list		args;
	va_list		args_copy;

	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	ft_vprintf_fd(fd, format, args_copy);
	va_end(args_copy);
	va_end(args);
}
