/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_dol_closed.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 20:40:04 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_heredoc_dollar_closed(char *no_quotes_lex, int fd)
{
	t_handle_vars	l_vars;
	char			*rl_input;
	char			*buffer_start;
	char			*contents;
	int				control_result;

	if (initialize_vars_pipe_hd(&rl_input, &contents, &l_vars) != 0)
		return (1);
	if (get_stdin())
		return (1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		control_result = check_hd_p(rl_input, &l_vars,
				no_quotes_lex, contents);
		if (control_result == 0)
			break ;
		else if (control_result == 1)
			return (control_result);
		buffer_start = rl_input;
		if (pipe_heredoc_get_content(&contents, rl_input, buffer_start))
			return (free(l_vars.buffer), 1);
	}
	return (write_and_free_contents(fd, contents), free(l_vars.buffer), 0);
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
			return (print_err(1, 2, "my(s)hell: malloc error 34\n"), NULL);
		ft_strcpy(contents, buffer);
	}
	else
	{
		old_size = ft_strlen(contents) + 1;
		new_size = old_size + ft_strlen(buffer) + 1;
		temp = ft_realloc(contents, old_size, new_size);
		if (temp == NULL)
			return (free(contents), print_err(1, 2,
					"my(s)hell: malloc error 35\n"), NULL);
		contents = temp;
		ft_strcat(contents, buffer);
	}
	return (contents);
}

int	pipe_heredoc_get_content(char **contents, char *buffer, char *buffer_start)
{
	*contents = get_heredoc_content(*contents, buffer);
	if (*contents == NULL)
	{
		if (buffer_start)
			free(buffer_start);
		if (*contents)
			free(*contents);
		return (1);
	}
	ft_strcat(*contents, "\n");
	free(buffer_start);
	return (0);
}

void	free_bufs_contents(char *contents, char *buffer, char *rl_input)
{
	if (contents != NULL)
	{
		free(contents);
		contents = NULL;
	}
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	if (rl_input != NULL)
	{
		free(rl_input);
		rl_input = NULL;
	}
}
