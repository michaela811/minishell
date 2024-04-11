/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 16:46:24 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/07 09:52:59 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dest_size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;
	size_t	e;

	e = 0;
	i = ft_strlen(dst);
	dest_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dest_size == 0)
		return (src_len + dest_size);
	while (src[e] != '\0' && i + 1 < dest_size)
	{
		dst[i] = src[e];
		i++;
		e++;
	}
	dst[i] = '\0';
	if (dest_size < dest_len)
		return (src_len + dest_size);
	else
		return (dest_len + src_len);
}
