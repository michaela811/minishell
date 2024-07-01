/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 11:37:03 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_there_here_doc(t_free_data *exec_data)
{
	t_p_tree *current = exec_data->tree->sibling;
	//t_p_tree *tree = current->child;

	printf("The sibling is %s\n", current->data->lexeme);
	printf("The sibling is %d\n", current->data->type);
	//printf("The sibling is %d\n", current->child->data->type);
	while (current != NULL)
	{
		if (current->data != NULL)
		{
			printf("Lexeme is %s\n", current->data->lexeme);
			if (current != NULL && current->data->type == HERE_DOC)
			{
				printf("child lexeme is %s\n", current->data->lexeme);
				execute_node(exec_data);
			}
		}
		current = current->sibling;
	}
}
