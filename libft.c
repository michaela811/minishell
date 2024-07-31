#include "minishell.h"

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

static int	ft_numlen(int num)
{
	int	count;

	count = 0;
	if (num == -2147483648 || num == 2147483647)
	{
		count = 10;
		return (count);
	}
	if (num == 0)
		count = 1;
	while (num)
	{
		num = num / 10;
		count++;
	}
	return (count);
}

static int	ft_is_negative(int n)
{
	int	minus;

	minus = 0;
	if (n < 0)
		minus = 1;
	return (minus);
}

static char	*ft_digits(void)
{
	char	*digits;

	digits = "0123456789";
	return (digits);
}

char	*ft_itoa(int n)
{
	char	*num_str;
	size_t	len;
	int		minus;

	minus = ft_is_negative(n);
	if (n < 0)
		n = n * (-1);
	len = ft_numlen(n) + minus;
	num_str = malloc((sizeof(char) * (len + 1)));
	if (num_str == NULL)
		return (NULL);
	if (n == -2147483648)
	{
		num_str = ft_strcpy(num_str, "-2147483648");
		return (num_str);
	}
	num_str[len] = '\0';
	while (len-- > (unsigned int)minus)
	{
		num_str[len] = ft_digits()[n % 10];
		n /= 10;
	}
	if (minus == 1)
		num_str[len] = '-';
	return (num_str);
}

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

char	*ft_strdup(const char *s)
{
	char	*copy;
	size_t	len;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	while (s[i])
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

/* char	*ft_strjoin(char const *s1, char const *s2)
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
} */
