#include "minishell.h"

void	is_there_here_doc(t_free_data *exec_data)
{
	t_p_tree *current = exec_data->tree->sibling;
	t_p_tree *tree = current->child;

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
