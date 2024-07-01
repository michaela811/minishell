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

t_token_list	*create_token(enum e_token_type type, char *lexeme)
{
	t_token			*new_token;
	t_token_list	*new_list_node;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("Failed to allocate memory for token");
		exit(MEMORY_ERROR);
	}
	new_token->lexeme = ft_strdup(lexeme);
	new_token->type = type;
	new_list_node = malloc(sizeof(t_token_list));
	if (!new_list_node)
	{
		free_token(new_token);
		perror("Failed to allocate memory for token list node");
		exit(MEMORY_ERROR);
	}
	new_list_node->token = new_token;
	new_list_node->next = NULL;
	return (new_list_node);
}

void	add_token(t_token_list **list, enum e_token_type type, char *lexeme)
{
	t_token_list	*new_node;
	t_token_list	*current;

	new_node = create_token(type, lexeme);
	if (!new_node)
	{
		perror("Failed to create token");
		exit(MEMORY_ERROR);
	}
	if (!*list)
		*list = new_node;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
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
