#include "libft.h"

size_t	handle_quotes(char *str, int *error)
{
	size_t	len;
	char	c;

	len = 0;
	if (!str)
		return (len);
	c = *str;
	while (*str)
	{
		str++;
		len++;
		if (*str == c)
			break ;
	}
	if (*str == c)
	{
		*error = 0;
		len++;
	}
	else
		*error = 1;
	return (len);
}

char	*update_pointer(char *str)
{
	if (str && *str)
	{
		*str = '\0';
		return (str + 1);
	}
	else
		return (str);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*i;
	char		*result;

	result = str;
	if (str)
		i = str;
	while (i && *i && ft_strchr(delim, *i))
		i++;
	str = i;
	while (str && *str)
	{
		result = str;
		while (str && !ft_strchr(delim, *str))
			str++;
		i = update_pointer(str);
	}
	return (result);
}
