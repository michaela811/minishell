/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_heredoc_1.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:06:47 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/08 16:46:16 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_lexeme(char *lexeme, char *quote, char *result)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (lexeme[i] != '\0')
	{
		if (lexeme[i] == '"' || lexeme[i] == '\'')
		{
			*quote = lexeme[i++];
			while (lexeme[i] != '\0' && lexeme[i] != *quote)
				result[j++] = lexeme[i++];
			if (lexeme[i] != *quote)
				return (free(result), print_err(1, 2,
						"my(s)hell: syntax error\n"), 1);
			i++;
		}
		else
			result[j++] = lexeme[i++];
	}
	result[j] = '\0';
	return (0);
}

int	remove_quotes(char **lexeme_ptr)
{
	char	*lexeme;
	char	*result;
	char	quote;

	lexeme = *lexeme_ptr;
	result = malloc(ft_strlen(lexeme) + 1);
	if (!result)
		return (print_err(1, 2, "my(s)hell: malloc error 1\n"), 1);
	if (handle_lexeme(lexeme, &quote, result))
		return (g_last_exit_status);
	free(*lexeme_ptr);
	*lexeme_ptr = result;
	return (0);
}

int	process_heredoc_dollar_closed(int file, char *no_quotes_lex)
{
	char	*buffer;

	while (1)
	{
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			return (print_err(1, 2, "my(s)hell: readline failure\n"), 1);
		if (break_heredoc(buffer))
			return (1);
		if (ft_exact_strcmp(buffer, no_quotes_lex) == 0)
		{
			free(buffer);
			return (0);
		}
		write(file, buffer, ft_strlen(buffer));
		write(file, "\n", 1);
		free(buffer);
	}
	return (0);
}

int	break_heredoc(char *buffer)
{
	if (g_last_exit_status == 130)
	{
		if (buffer != NULL)
			free(buffer);
		return (1);
	}
	return (0);
}

int	process_heredoc_dollar_open(int file, t_exec_vars *vars,
		t_free_data *exec_data, char *no_quotes_lex)
{
	char	*rl_input;
	//char	buffer_no_dollar[1024];
	char	*buffer_start;
	t_handle_vars	l_vars;

	l_vars.buffer_size = 4096;
	l_vars.buffer = malloc(l_vars.buffer_size);
	if (l_vars.buffer == NULL)
		return (print_err(1, 2, "my(s)hell: malloc error x1"), 1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		if (rl_input == NULL)
			return (print_err(1, 2, "my(s)hell: readline failure\n"),
				free(l_vars.buffer), 1);
		if (break_heredoc(rl_input))
			return (free(l_vars.buffer), 1);
		if (ft_exact_strcmp(rl_input, no_quotes_lex) == 0)
			return (free(rl_input), free(l_vars.buffer), 0);
		buffer_start = rl_input;
		if (handle_dollar_error(&rl_input, &l_vars, vars, exec_data))
			return (free(buffer_start), free(l_vars.buffer), 1);
		write(file, l_vars.buffer, ft_strlen(l_vars.buffer));
		write(file, "\n", 1);
		free(buffer_start);
		free(l_vars.buffer);
	}
	return (0);
}