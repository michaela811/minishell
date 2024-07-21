/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:30 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:33:32 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *str)
{
	perror(str);
	return (1);
}

void	execve_error(char **s_cmd)
{
	print_err(138, 2, "Execve failed\n");
	ft_putendl_fd("This command was not found: ", 2);
	ft_putendl_fd(s_cmd[0], 2);
	free_array(s_cmd);
	exit(0);
}

void	print_err(int status, unsigned int fd, char *format, ...)
{
	va_list	args;
	va_list	args_copy;

	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	ft_vprintf_fd(fd, format, args_copy);
	va_end(args_copy);
	va_end(args);
}
