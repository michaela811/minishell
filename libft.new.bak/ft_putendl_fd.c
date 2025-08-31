/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:04:18 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:10:26 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <unistd.h>
#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	while (*s)
	{
		(write (fd, s++, 1));
	}
	write (fd, "\n", 1);
}
/*
int main()
{
    char *c = "ddd";
    ft_putendl_fd(c, 1);
}
*/
