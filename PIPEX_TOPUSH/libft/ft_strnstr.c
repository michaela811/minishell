/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 13:58:01 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/07 16:14:17 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	e;

	if (*little && *big && len == 0)
		return (NULL);
	if (little == big)
		return ((char *)big);
	i = 0;
	e = 0;
	while (big[e])
	{
		i = 0;
		while (big[e] == little[i] && big[e] && e < len)
		{
			i++;
			e++;
		}
		if (little[i] == '\0')
			return ((char *)&big[e - i]);
		e = (e - i) + 1;
	}
	return (NULL);
}
