/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:50:17 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/15 22:10:35 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <unistd.h>
#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	while (*s)
		(write (fd, s++, 1));
}
/*
int main()
{
    char *c = "ddd";
    ft_putstr_fd(c, 1);
}
*/
