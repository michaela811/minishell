/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:34:03 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 10:51:20 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	handle_quotes_in_heredoc(char *input, char *result, int *i, int *j)
{
	char	quote;

	if (input[*i] == '"' || input[*i] == '\'')
	{
		quote = input[(*i)++];
		while (input[*i] != '\0' && input[*i] != quote)
			result[(*j)++] = input[(*i)++];
		if (input[*i] != quote)
		{
			g_last_exit_status = 1;
			return (1);
		}
		(*i)++;
	}
	return (0);
}

int	malloc_result(char **result, char *input, int *error)
{
	*result = malloc(ft_strlen(input) + 1);
	if (!(*result))
	{
		*error = 1;
		return (1);
	}
	return (0);
}

char	*handle_quotes_heredoc(char *input, int *error)
{
	char	*result;
	int		i;
	int		j;

	if (malloc_result(&result, input, error))
		return (print_err(1, 2, "echo: memory allocation\n"), NULL);
	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if (handle_quotes_in_heredoc(input, result, &i, &j))
		{
			*error = 1;
			return (free(result), NULL);
		}
		else if (input[i] == '$' && input[i + 1] == '?')
			return_exit_status(result, &i, &j, error);
		if (*error)
			return (free(result), NULL);
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

void	return_exit_status(char *result, int *i, int *j, int *error)
{
	char	*temp;

	temp = ft_itoa(g_last_exit_status);
	if (temp == NULL)
	{
		*error = 1;
		print_err(1, 2, "echo: memory allocation\n");
		return ;
	}
	result[*j] = temp[0];
	*j = *j + ft_strlen(temp);
	*i += 2;
	free(temp);
	return ;
}
