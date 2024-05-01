#include "libft.h"

size_t handle_quotes(char *str, int *error)
{
	//char *result;
	size_t len = 0;
	char	*c;

	if (!str)//WHY?
		return (len);
		//str++;
	//result = str;
	c = str;
	while(*str)
	{
		str++;
		len++;
		if (*str == *c)
			break;
	}
	if (*str == *c)
	{
		*error = 0;
		len++;
	}
	else
		*error = 1;
	return (len);
}

char *update_pointer(char *str)
{
    if (str && *str)
	{
        *str = '\0';
        return (str + 1);
	}
    else
        return str;
}

char	*ft_strtok(char *str, const char *delim, int *error)
{
	static char	*i;
	char		*result;
	size_t len;

	result = str;
	if (str)
		i = str;
	if (i && (*i == '"' || *i == '\''))
	{
		len = handle_quotes(i, error);
		result = i;
		i += len;
		str = i;
		i = update_pointer(str);
		return (result);
	}
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