/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/05/23 16:48:41 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int			minus;
	long long int	number;

	number = 0;
	minus = 0;
	while (*nptr && (*nptr == 32 || (*nptr >= 9 && *nptr <= 13)))
		nptr++;
	if (*nptr == '-')
		minus++;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	if (ft_strcmp(nptr, "9223372036854775808") == 0 && minus == 1)
		return (INT_MIN);
	if ((*nptr <= 47) || (*nptr >= 58 && *nptr < 127))
		return (0);
	while (*nptr >= '0' && *nptr <= '9')
	{
		if (number > (LLONG_MAX - (*nptr - '0')) / 10)
			return (INT_MAX);
		number = number * 10 + (*nptr - '0');
		nptr++;
	}
	if (minus == 1 && number != 0)
		return (-number);
	return (number);
}
