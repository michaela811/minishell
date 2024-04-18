/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:40:53 by mmasarov          #+#    #+#             */
/*   Updated: 2023/09/12 13:03:16 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = s2[i];
	return (s1);
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
