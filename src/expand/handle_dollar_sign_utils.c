/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_sign_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 11:40:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 20:39:23 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_first_non_alnum(char *str)
{
	while (*str)
	{
		if (!isalnum((unsigned char)*str) && *str != '_')
			return (str);
		str++;
	}
	return (NULL);
}

int	resize_buffer(char **buffer, int *buffer_size)
{
	int		new_buf_size;
	char	*new_buffer;

	new_buf_size = (*buffer_size) * 2;
	new_buffer = ft_realloc(*buffer, *buffer_size, new_buf_size);
	if (new_buffer == NULL)
	{
		print_err(1, 2, "my(s)hell: malloc error 8\n");
		g_last_exit_status = 1;
		return (1);
	}
	*buffer = new_buffer;
	*buffer_size = new_buf_size;
	return (0);
}

int	ft_strlcat_custom(char *dst, const char *src, int dest_size)
{
	int	dest_len;
	int	src_len;
	int	i;
	int	e;

	e = 0;
	i = ft_strlen(dst);
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
	dest_len = ft_strlen(dst);
	if (dest_size <= dest_len + 1)
		return (-1);
	else
		return (dest_len);
}

char	*ft_strncat_custom(char *dest, const char *src, size_t n)
{
	size_t	dest_len;
	size_t	i;

	i = 0;
	dest_len = 0;
	while (dest[dest_len] != '\0')
		dest_len++;
	while (i < n && src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

int	question_mark(char **start, char *buffer, char *dollar,
		int dollar_status)
{
	char	*exit_status;

	exit_status = ft_itoa(dollar_status);
	if (exit_status == NULL)
		return (print_err(1, 2, "my(s)hell: malloc in ft_itoa\n"), 1);
	if (!check_null(exit_status, &g_last_exit_status))
		return (1);
	ft_strcat(buffer, exit_status);
	free(exit_status);
	*start = dollar + 2;
	return (0);
}
