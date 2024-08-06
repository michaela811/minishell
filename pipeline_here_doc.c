/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 14:11:08 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_stdin(void)
{
	int				tty_fd;
	struct termios	term;

	if (isatty(STDIN_FILENO) == 0)
		return (0);
	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		tty_fd = open("/dev/tty", O_RDONLY);
		if (dup2(tty_fd, STDIN_FILENO) == -1)
			return (print_err(errno, 2,
					"my(s)hell: dup2 tty_fd to STDIN_FILENO"),
				close(tty_fd), 1);
		close(tty_fd);
	}
	return (0);
}

char	*get_heredoc_content(char *contents, char *buffer)
{
	char	*temp;
	size_t	old_size;
	size_t	new_size;

	temp = NULL;
	if (contents == NULL)
	{
		contents = malloc(ft_strlen(buffer) + 2);
		if (contents == NULL)
			return (free(buffer), NULL);
		ft_strcpy(contents, buffer);
	}
	else
	{
		old_size = ft_strlen(contents) + 1;
		new_size = old_size + ft_strlen(buffer) + 1;
		temp = ft_realloc(contents, old_size, new_size);
		if (temp == NULL)
			return (free(contents), free(buffer), NULL);
		contents = temp;
		ft_strcat(contents, buffer);
	}
	return (contents);
}

int	is_it_delimiter(char *node, char *buffer)
{
	if (ft_exact_strcmp(buffer, node) == 0)
	{
		free(buffer);
		return (1);
	}
	return (0);
}
