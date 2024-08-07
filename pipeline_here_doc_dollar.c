/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_dollar.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:03:19 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/07 18:25:00 by mmasarov         ###   ########.fr       */
/*   Updated: 2024/08/07 12:02:20 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	break_pipe_heredoc(char *buffer, char *contents)
{
	if (g_last_exit_status == 130)
	{
		if (contents != NULL)
			free(contents);
		free(buffer);
		return (1);
	}
	return (0);
}

int	write_and_free_contents(int fd, char *contents)
{
	if (contents != NULL)
	{
		write(fd, contents, ft_strlen(contents));
		free(contents);
	}
	return (0);
}

int	readline_check(char *buffer)
{
	if (buffer == NULL)
	{
		print_err(1, 2, "my(s)hell: malloc error 32");
		return (1);
	}
	return (0);
}

int	process_buffer(char *buffer, char *buffer_no_dollar, t_free_data *exec_data,
	char *buffer_start)
{
	if (handle_dollar_sign(&buffer, buffer_no_dollar, exec_data,
			sizeof(buffer_no_dollar)))
	{
		free(buffer_start);
		return (1);
	}
	return (0);
}

int	pipe_heredoc_dollar_open(char *no_quotes_lex, int fd,
		t_free_data *exec_data)
{
	char	*buffer;
	char	buffer_no_dollar[1024];
	char	*buffer_start;
	char	*contents;

	buffer = NULL;
	contents = NULL;
	if (get_stdin())
		return (1);
	while (1)
	{
		buffer = readline("heredoc> "); // Try to fail!
		if (break_pipe_heredoc(buffer, contents))
			return (1);
		if (buffer == NULL)
		{
			g_last_exit_status = 1;
			break ;
		}
		if (is_it_delimiter(no_quotes_lex, buffer))
			break ;
		buffer_start = buffer;
		if (handle_dollar_sign(&buffer, buffer_no_dollar, exec_data,
				sizeof(buffer_no_dollar)))
			return (free(buffer_start), 1);
		if (pipe_heredoc_get_content(&contents, buffer_no_dollar, buffer_start))
			return (1);
	}
	return write_and_free_contents(fd, contents);
}

int	pipe_heredoc_dollar_closed(char *no_quotes_lex, int fd)
{
	char	*buffer;
	char	*buffer_start;
	char	*contents;

	buffer = NULL;
	contents = NULL;
	if (get_stdin())
		return (1);
	while (1)
	{
		buffer = readline("heredoc> ");
		if (break_pipe_heredoc(buffer, contents))
			return (1);
		if (buffer == NULL)
		{
			g_last_exit_status = 1;
			break ;
		}
		if (is_it_delimiter(no_quotes_lex, buffer))
			break ;
		buffer_start = buffer;
		if (pipe_heredoc_get_content(&contents, buffer, buffer_start))
			return (1);
	}
	return write_and_free_contents(fd, contents);
}