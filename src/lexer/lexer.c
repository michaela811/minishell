/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:34 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/09 16:03:20 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_additional_chars(char *input, const char *delim)
{
	int	num_of_extra;

	num_of_extra = 0;
	while (*input)
	{
		if (ft_strchr(delim, *input) != NULL)
		{
			if ((*input == '<' && *(input + 1) == '<') || (*input == '>'
					&& *(input + 1) == '>'))
				input++;
			num_of_extra += 2;
		}
		input++;
	}
	return (num_of_extra);
}

char	*preprocess_input(char *str, const char *delim)
{
	int		original_len;
	int		additional_chars;
	char	*preprocessed;
	char	*dest;

	original_len = ft_strlen(str);
	additional_chars = count_additional_chars(str, delim);
	preprocessed = malloc(original_len + additional_chars + 1);
	if (preprocessed == NULL)
		return (print_err(1, 2, "Memory allocation error\n"), NULL);
	dest = preprocessed;
	process_input_str(str, delim, dest);
	return (preprocessed);
}

static void	handle_delim_char(char **dest, char **str)
{
	if (**str == ' ' || **str == '\t')
	{
		*(*dest)++ = -1;
		*(*dest)++ = -1;
	}
	else if ((**str == '<' && *(*str + 1) == '<') || (**str == '>'
			&& *(*str + 1) == '>'))
	{
		*(*dest)++ = -1;
		*(*dest)++ = **str;
		*(*dest)++ = (*++(*str));
		*(*dest)++ = -1;
	}
	else
	{
		*(*dest)++ = -1;
		*(*dest)++ = **str;
		*(*dest)++ = -1;
	}
}

void	process_input_str(char *str, const char *delim, char *dest)
{
	int		in_quotes;
	char	c;

	in_quotes = 0;
	c = '\0';
	while (*str != '\0')
	{
		if (*str == '"' || *str == '\'')
		{
			if (!in_quotes)
				c = *str;
			if (!c || c == *str)
				in_quotes = !in_quotes;
		}
		if (in_quotes == 0 && ft_strchr(delim, *str) != NULL)
			handle_delim_char(&dest, &str);
		else
			*dest++ = *str;
		str++;
	}
	*dest = '\0';
	if (in_quotes)
		print_err(2, 2, "my(s)hell: unclosed quotes\n");
}
