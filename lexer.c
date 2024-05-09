#include "lexer.h"

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
	preprocessed = MY_MALLOC(original_len + additional_chars + 1);
	if (preprocessed == NULL)
		return (perror("Memory allocation error"), NULL);
	dest = preprocessed;
	process_input_str(str, delim, dest);
	//printf("Preprocessed input is %s\n", dest);
	return (preprocessed);
}

void	process_input_str(char *str, const char *delim, char *dest)
{
	int in_quotes = 0;
	char c;

	c = '\0';
	while (*str != '\0')
	{
		if (*str == '"' || *str == '\'')
		{
			if (!c || c == *str)
            	in_quotes = !in_quotes;
			if (!c)
				c = *str;
		}
		if (in_quotes == 0 && ft_strchr(delim, *str) != NULL)
		{
			if (*str == ' ' )
			{
				*dest++ = '@';
				*dest++ = '@';
			}
			else if ((*str == '<' && *(str + 1) == '<') || (*str == '>'
					&& *(str + 1) == '>'))
			{
				*dest++ = '@';
				*dest++ = *str;
				*dest++ = *++str;
				*dest++ = '@';
			}
			else
			{
				*dest++ = '@';
				*dest++ = *str;
				*dest++ = '@';
			}
		}
		else
			*dest++ = *str;
		str++;
	}
	*dest = '\0';
}

enum token_type	determine_token_type(char *token_value)
{
	if (strcmp(token_value, "|") == 0) // ad this in libft library
		return (PIPE);
	else if (strcmp(token_value, ">") == 0)
		return (RED_TO);
	else if (strcmp(token_value, "<") == 0)
		return (RED_FROM);
	else if (strcmp(token_value, ">>") == 0)
		return (APPEND);
	else if (strcmp(token_value, "<<") == 0)
		return (HERE_DOC);
	else
		return (WORD);
}

void	handle_memory_error(t_token **token_list, int num_tokens)
{
	int	i;

	i = 0;
	while (i < num_tokens)
	{
		free((*token_list)[i].lexeme);
		i++;
	}
	free(*token_list);
	perror("Memory allocation error");
	exit(EXIT_FAILURE);//SHOULD BE REPLACED WITH INT?
}
