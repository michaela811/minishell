/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:38:18 by mmasarov         ###   ########.fr       */
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

void	print_p_tree(t_p_tree *tree, int depth)
{
	int	i;

	i = 0;
	if (!tree)
		return ;
	while (i++ < depth)
		printf("  ");
	if (tree->data)
		printf("%s (%d)\n", tree->data->lexeme, tree->data->type);
	else
		printf("NULL\n");
	print_p_tree(tree->child, depth + 1);
	print_p_tree(tree->sibling, depth);
}
