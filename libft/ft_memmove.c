/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 14:11:01 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/13 15:53:55 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	d = (char *)dest;
	s = (char *)src;
	i = 0;
	if (src < dest)
	{
		while (n--)
		{
			d[n] = s[n];
		}
	}
	else if (src > dest)
	{
		while (n--)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}
