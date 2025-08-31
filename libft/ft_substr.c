/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:14:56 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/11 10:15:23 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_special_cases(char const *s, unsigned int start, size_t len)
{
	size_t	lenght;

	lenght = ft_strlen(s);
	if (lenght < start)
		len = 0;
	if ((lenght + start) < len)
		len = lenght + start;
	if (start == (lenght - 1) && len != 0)
		len = 1;
	if ((lenght - start) < len)
		len = lenght - start;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	len = ft_special_cases(s, start, len);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
