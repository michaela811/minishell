#include "lexer.h"

t_parse_tree	*alloc_parse_tree(void)
{
	t_parse_tree	*tree;

	tree = MY_MALLOC(sizeof(t_parse_tree));
	if (tree == NULL)
		return (NULL);
	tree->data = NULL;
	tree->child = NULL;
	tree->sibling = NULL;
	return (tree);
}

void	link_node(t_parse_tree **current, t_parse_tree *new_node)
{
	t_parse_tree	*last;

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

void	link_pipe(t_parse_tree **current, t_parse_tree *new_node)
{
	t_parse_tree	*last;

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

t_token_list	*create_token(enum token_type type, char *lexeme)
{
	t_token			*new_token;
	t_token_list	*new_list_node;

	new_token = MY_MALLOC(sizeof(t_token));
	if (!new_token)
	{
		perror("Failed to allocate memory for token");
		exit(MEMORY_ERROR);//SHOULD BE REPLACED WITH INT?
	}
	new_token->lexeme = ft_strdup(lexeme);
	new_token->type = type;
	new_list_node = MY_MALLOC(sizeof(t_token_list));
	if (!new_list_node)
	{
		free_token(new_token);
		perror("Failed to allocate memory for token list node");
		exit(MEMORY_ERROR);//SHOULD BE REPLACED WITH INT?
	}
	new_list_node->token = new_token;
	new_list_node->next = NULL;
	return (new_list_node);
}

void	add_token(t_token_list **list, enum token_type type, char *lexeme)
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

//TO BE DELETED
void	print_parse_tree(t_parse_tree *tree, int depth)
{
	int	i;

	i = 0;
	if (!tree)
		return ;
	while (i++ < depth)
		ft_printf("  ");
	if (tree->data)
		ft_printf("%s (%d)\n", tree->data->lexeme, tree->data->type);
	else
		ft_printf("NULL\n");
	print_parse_tree(tree->child, depth + 1);
	print_parse_tree(tree->sibling, depth);
}
