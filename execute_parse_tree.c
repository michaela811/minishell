/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_parse_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 13:54:41 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_parse_tree(t_free_data *exec_data)
{
	if (exec_data->tree == NULL)
		return (0);
	if (exec_data->tree->sibling)
	{
		g_last_exit_status = 0;
		execute_pipeline(exec_data);
	}
	else
		execute_node(exec_data);
	return (g_last_exit_status);
}
