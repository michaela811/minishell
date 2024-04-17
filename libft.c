#include "lexer.h"

static void	ft_free(char **array, int j)
{
	while (--j >= 0)
		free(array[j]);
	free(array);
}

static size_t	n_words(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] != c)
		{
			count++;
			while (str[i] != c && str[i])
				i++;
		}
		else
			i++;
	}
	return (count);
}

static size_t	size_word(const char *s, char c, int i)
{
	size_t	size;

	size = 0;
	while (s[i] != c && s[i])
	{
		size++;
		i++;
	}
	return (size);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	i;
	size_t	j;
	size_t	size;

	i = 0;
	j = -1;
	array = malloc(sizeof(char *) * (n_words(s, c) + 1));
	if (!array)
		return (NULL);
	while (++j < n_words(s, c))
	{
		while (s[i] == c)
			i++;
		size = size_word(s, c, i);
		array[j] = ft_substr(s, i, size);
		if (!array[j])
		{
			ft_free(array, j);
			return (NULL);
		}
		i = i + size;
	}
	array[j] = 0;
	return (array);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	int		i;
	int		x;

	i = 0;
	x = 0;
	new_str = (char *)malloc(sizeof(char)
			* (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new_str)
		return (NULL);
	while (s1[i])
	{
		new_str[x] = s1[i];
		x++;
		i++;
	}
	i = 0;
	while (s2[i])
	{
		new_str[x] = s2[i];
		x++;
		i++;
	}
	new_str[x] = '\0';
	return (new_str);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write (fd, &s[i++], 1);
	}
	write (fd, "\n", 1);
}

static size_t	ft_special_cases(char const *s, unsigned int start, size_t len)
{
	size_t	lenght;

	lenght = ft_strlen(s);
	if (lenght < start)
		len = 0;
	if ((lenght + start) < len)
		len = lenght + start;
	if (start == (lenght - 1) && len != 0)
		len = 1;
	if ((lenght - start) < len)
		len = lenght - start;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	len = ft_special_cases(s, start, len);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
