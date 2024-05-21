/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 19:03:30 by dpadenko          #+#    #+#             */
/*   Updated: 2024/05/21 11:29:40 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_gnl(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

void	my_free(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}

char	*ft_strjoin_gnl(char *read_str, char *buff)
{
	size_t	i;
	size_t	j;
	char	*str;

	if ((ft_strlen_gnl(read_str) + ft_strlen_gnl(buff)) == 0)
		return (NULL);
	str = malloc(sizeof(char) * ((ft_strlen_gnl(read_str) + ft_strlen_gnl(buff)) + 1));
	if (str == NULL)
		return (my_free(&read_str), my_free(&buff), NULL);
	i = 0;
	j = 0;
	while (read_str && read_str[i] != '\0')
	{
		str[i] = read_str[i];
		i++;
	}
	while (buff[j] != '\0')
		str[i++] = buff[j++];
	str[i] = '\0';
	return (my_free(&read_str), my_free(&buff), str);
}
