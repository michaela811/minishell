/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 19:46:57 by dpadenko          #+#    #+#             */
/*   Updated: 2023/09/20 11:19:22 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
#include "libft.h"

static long int	ft_count(int n)
{
	int			count;

	count = 0;
	if (n == -2147483648)
		return (10);
	if (n < 0)
		n = -n;
	while ((n / 10) > 0)
	{
		count++;
		n = n / 10;
	}
	return (count + 1);
}

static char	*i_to_a(long int n, char *s)
{
	static int	i = 0;

	if (n == -2147483648)
	{
		ft_strlcpy(s, "-2147483648", 12);
		return (s);
	}
	if (n / 10 != 0)
		i_to_a(n / 10, s);
	else if (n < 0)
		i = 1;
	else
		i = 0;
	if (n < 0)
	{
		s[0] = '-';
		n = -n;
	}
	s[i++] = (n % 10) + '0';
	s[i] = '\0';
	return (s);
}

char	*ft_itoa(int n)
{
	int		negative;
	char	*i_str;

	negative = 0;
	if (n < 0)
		negative = 1;
	i_str = malloc(ft_count(n) + negative + 1);
	if (i_str == NULL)
		return (NULL);
	return (i_to_a(n, i_str));
}
/*
int main()
{
	int i = -2147483648;

	printf("%s\n", ft_itoa(i));
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return(0);
	printf("%s\n", ft_itoa(atoi(argv[1])));
}
*/
