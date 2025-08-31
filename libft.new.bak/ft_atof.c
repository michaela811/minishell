/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 15:39:08 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	return_float_part(char *s, double float_part)
{
	double	d;

	d = 1.0;
	while (ft_isdigit(*s))
	{
		d /= 10.0;
		float_part += (*s++ - '0') * d;
	}
	return (float_part);
}

void	init_values(long *int_part, double *float_part, int *minus)
{
	*int_part = 0;
	*float_part = 0.0;
	*minus = 1;
}

long	return_int_part(char **s, long int_part)
{
	int		lenght;

	lenght = 0;
	while (ft_isdigit(**s))
	{
		lenght++;
		if (lenght > 19)
			return (INT_MAX);
		int_part = int_part * 10 + (**s - '0');
		(*s)++;
	}
	return (int_part);
}

double	ft_atof(char *s)
{
	long	int_part;
	double	float_part;
	int		minus;
	double	result;

	init_values(&int_part, &float_part, &minus);
	while ((*s >= 9 && *s <= 13) || 32 == *s)
		s++;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			minus = -1;
	int_part = return_int_part(&s, int_part);
	if (int_part == INT_MAX)
		return (1.7976931348623158e+308);
	if (*s == '.')
		++s;
	float_part = return_float_part(s, float_part);
	result = (int_part + float_part) * minus;
	if (result > 1.7976931348623158e+308)
		return (1.7976931348623158e+308);
	if (result < -1.7976931348623158e+308)
		return (-1.7976931348623158e+308);
	return (result);
}