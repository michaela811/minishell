/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 11:19:53 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 11:19:54 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strpbrk(char *str, char *delim)
{
	while (*str)
	{
		if (ft_strchr(delim, *str))
			break ;
		str++;
	}
	if (*str)
		return (str);
	return (NULL);
}