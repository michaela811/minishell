/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 19:03:02 by dpadenko          #+#    #+#             */
/*   Updated: 2024/05/21 11:33:36 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

char	*del_buf_return_res(char **buffer, char *res, int len_to_del)
{
	int		new_len;
	int		i;
	char	*new_buffer;

	i = len_to_del;
	new_len = 0;
	while ((*buffer)[i++])
		new_len++;
	if (new_len == 0)
		return (my_free_gnl(buffer), res);
	new_buffer = (char *)malloc(new_len + 1);
	if (!new_buffer)
		return (free(res), my_free_gnl(buffer), NULL);
	i = 0;
	while ((*buffer)[len_to_del])
		new_buffer[i++] = (*buffer)[len_to_del++];
	new_buffer[i] = '\0';
	my_free_gnl(buffer);
	*buffer = new_buffer;
	return (res);
}

char	*extract_til_nl_or_end(char **buff)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	if (!*buff)
		return (NULL);
	while ((*buff) && (*buff)[i])
	{
		if ((*buff)[i++] == '\n')
			break ;
	}
	res = (char *)malloc(i + 1);
	if (!res)
		return (my_free_gnl(buff), NULL);
	j = 0;
	while (j < i)
	{
		res[j] = (*buff)[j];
		j++;
	}
	res[j] = '\0';
	return (del_buf_return_res(buff, res, i));
}

char	*read_buff_size(int fd, char **buffer)
{
	int		read_bytes;
	char	*temp;

	temp = (char *)malloc(BUFFER_SIZE + 1);
	if (!temp)
		return (my_free_gnl(buffer), NULL);
	read_bytes = read(fd, temp, BUFFER_SIZE);
	if (read_bytes == 0)
		return (free(temp), NULL);
	if (read_bytes < 0)
		return (free(temp), my_free_gnl(buffer), NULL);
	temp[read_bytes] = '\0';
	return (temp);
}

int	check_nl(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*buff;
	char		*read_bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (check_nl(buff))
			return (extract_til_nl_or_end(&buff));
		read_bytes = read_buff_size(fd, &buff);
		if (!read_bytes && buff)
			return (extract_til_nl_or_end(&buff));
		else if (!read_bytes)
			return (NULL);
		buff = ft_strjoin_gnl(buff, read_bytes);
	}
}

void	ft_putendl_fd_gnl(char *s, int fd)
{
	if (s)
	{
		write(fd, s, ft_strlen_gnl(s));
		write(fd, "\n", 1);
	}
}

/* int		main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc == 1)
		fd = 0;
	else if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	else
		return (2);
	while ((line = get_next_line(fd)))
	{
		ft_putendl_fd(line, fd);
		free(line);
	}
	if (argc == 2)
		close(fd);
} */
