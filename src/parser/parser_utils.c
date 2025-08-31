/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 18:20:07 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_node(t_p_tree **current, t_p_tree *new_node)
{
	t_p_tree	*last;

	if (current == NULL || new_node == NULL)
		return ;
	if (*current == NULL)
		*current = new_node;
	else
	{
		last = *current;
		while (last->child != NULL)
			last = last->child;
		last->child = new_node;
	}
}

void	link_pipe(t_p_tree **current, t_p_tree *new_node)
{
	t_p_tree	*last;

	if (current == NULL || new_node == NULL)
		return ;
	if ((*current)->sibling == NULL)
		(*current)->sibling = new_node;
	else
	{
		last = *current;
		while (last->sibling != NULL)
			last = last->sibling;
		last->sibling = new_node;
	}
}
