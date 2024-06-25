#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	char	*saved;

	saved = dest;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = 0;
	return (saved);
}
