/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_sign_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 11:40:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/09 12:22:53 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_first_non_alnum(char *str)
{
	while (*str)
	{
		if (!isalnum((unsigned char)*str) && *str != '_')
			return (str);
		str++;
	}
	return (NULL);
}
