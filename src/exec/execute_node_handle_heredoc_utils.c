/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_heredoc_1.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:06:47 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 11:29:32 by dpadenko         ###   ########.fr       */
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

int	malloc_check(char *no_quotes_lex, int *error)
{
	if (no_quotes_lex == NULL)
	{
		*error = 1;
		print_err(1, 2, "my(s)hell: malloc error 2\n");
		return (1);
	}
	return (0);
}
