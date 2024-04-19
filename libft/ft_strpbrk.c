#include "libft.h"

char *ft_strpbrk(char *str, char *delim)
{
    while (*str)
    {
        if (ft_strchr(delim, *str))
            break;
        str++;
    }
    if (*str)
        return (str);
    return (NULL);
}