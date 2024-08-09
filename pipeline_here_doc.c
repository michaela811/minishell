/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/09 20:35:00 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_stdin(void)
{
	int				tty_fd;
	struct termios	term;

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
			return (print_err(1, 2, "my(s)hell: malloc error 34"), NULL);
		ft_strcpy(contents, buffer);
	}
	else
	{
		old_size = ft_strlen(contents) + 1;
		new_size = old_size + ft_strlen(buffer) + 1;
		temp = ft_realloc(contents, old_size, new_size);
		if (temp == NULL)
			return (print_err(1, 2, "my(s)hell: malloc error 35"), NULL);
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

int	pipe_heredoc_dollar_closed(char *no_quotes_lex, int fd)
{
	char	*rl_input;
	char	*buffer_start;
	char	*contents;

	rl_input = NULL;
	contents = NULL;
	t_handle_vars	l_vars;

	l_vars.buffer_size = 4096;
	l_vars.buffer = malloc(l_vars.buffer_size);
	//buffer_no_dollar = malloc(4096);
	if (l_vars.buffer == NULL)
		return (print_err(1, 2, "my(s)hell: malloc error x1"),
			free(l_vars.buffer), 1);
	if (get_stdin())
		return (1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		if (break_pipe_heredoc(rl_input, contents))
			return (free(l_vars.buffer), 1);
		if (readline_check(rl_input))
			break ;
		if (is_it_delimiter(no_quotes_lex, rl_input))
			break ;
		//if (ft_strlen(rl_input) >= 4096)
			//return (print_err(1, 2, "my(s)hell: heredoc too long\n"),
				//free(rl_input), 1);
		while ((int)ft_strlen(rl_input) >= l_vars.buffer_size)
			if (resize_buffer(&l_vars.buffer, &l_vars.buffer_size))
				return (free_bufs_contents(contents, l_vars.buffer, rl_input), 1);
		buffer_start = rl_input;
		if (pipe_heredoc_get_content(&contents, rl_input, buffer_start))
			return (1);
	}
	return (write_and_free_contents(fd, contents),free(l_vars.buffer), 0);
}

int	pipe_heredoc_get_content(char **contents, char *buffer, char *buffer_start)
{
	*contents = get_heredoc_content(*contents, buffer);
	if (*contents == NULL)
	{
		if (buffer_start)
			free(buffer_start);
		if (contents)
			free(contents);
		print_err(1, 2, "my(s)hell: malloc error in get_heredoc_content");
		return (1);
	}
	ft_strcat(*contents, "\n");
	free(buffer_start);
	return (0);
}
