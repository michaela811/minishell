/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:12:53 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/07 17:08:00 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars)
{
	*str1 = ft_strdup(*str2);
	if (!check_null(*str1, &vars->error))
		print_err(1, 2, "my(s)hell: malloc error\n");
}
