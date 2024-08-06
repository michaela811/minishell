/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:12:53 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/06 15:13:20 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars)
{
	*str1 = ft_strdup(*str2);
	check_null(*str1, &vars->error);
}
