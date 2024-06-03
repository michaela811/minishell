/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:15:44 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/13 15:52:29 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	int		i;
	int		x;

	i = 0;
	x = 0;
	if (!s1)
		return (ft_strdup(s2));
    if (!s2)
		return (ft_strdup(s1));
	newstr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!newstr)
		return (NULL);
	while (s1[i])
		newstr[x++] = s1[i++];
	i = 0;
	while (s2[i])
		newstr[x++] = s2[i++];
	newstr[x] = '\0';
	return (newstr);
}
