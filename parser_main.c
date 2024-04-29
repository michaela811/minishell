#include "lexer.h"
/*
int is_pipe_sequence(t_token_list **tok, t_parse_tree **new)//, int *status)
{
	t_parse_tree *current_command;
	t_parse_tree *pipe_node;
	t_parse_tree *next_command;

    if (*tok == NULL || (*tok)->token == NULL)
        return(SYNTAX_ERROR);
    *new = alloc_parse_tree();
    if (*new == NULL)
        return(MEMORY_ERROR);
    current_command = NULL;
    if(is_simple_command(tok, &current_command) != SUBTREE_OK || !current_command->child)
        return(free_parse_tree(*new), SYNTAX_ERROR); //COULD BE ALSO MEMORY_ERROR
    (*new)->child = current_command;
    while (*tok != NULL && (*tok)->token->type == PIPE)
    {
        pipe_node = alloc_parse_tree();
        if (pipe_node == NULL)
            return(free_parse_tree(*new), MEMORY_ERROR);
        pipe_node->data = (*tok)->token;
        *tok = (*tok)->next;
        link_pipe(new, pipe_node);
        next_command = NULL;
        if(is_simple_command(tok, &next_command) != SUBTREE_OK || !next_command->child)
            return(free_parse_tree(*new), SYNTAX_ERROR);
        link_pipe(new, next_command);//pipe_node->child = next_command;
    }
    return(SUBTREE_OK);
}*/

int	create_and_link_pipe(t_token_list **tok, t_parse_tree **new)
		//t_parse_tree *pipe_node, t_parse_tree *next_command)
{
	t_parse_tree	*pipe_node;
	t_parse_tree	*next_command;

	pipe_node = alloc_parse_tree();
	if (pipe_node == NULL)
		return (free_parse_tree(*new), MEMORY_ERROR);
	pipe_node->data = (*tok)->token;
	*tok = (*tok)->next;
	link_pipe(new, pipe_node);
	next_command = NULL;
	if (is_simple_command(tok, &next_command) != SUBTREE_OK
		|| !next_command->child)
		return (free_parse_tree(*new), SYNTAX_ERROR);
	link_pipe(new, next_command);
	return (SUBTREE_OK);
}

int	is_pipe_sequence(t_token_list **tok, t_parse_tree **new)
{
	t_parse_tree	*current_command;
	//t_parse_tree	*pipe_node;
	//t_parse_tree	*next_command;

	if (*tok == NULL || (*tok)->token == NULL)
		return (SYNTAX_ERROR);
	*new = alloc_parse_tree();
	if (*new == NULL)
		return (MEMORY_ERROR);
	current_command = NULL;
	if (is_simple_command(tok, &current_command) != SUBTREE_OK
		|| !current_command->child)
		return (free_parse_tree(*new), SYNTAX_ERROR); //COULD BE ALSO MEMORY_ERROR
	(*new)->child = current_command;
	while (*tok != NULL && (*tok)->token->type == PIPE)
	{
		if (create_and_link_pipe(tok, new) != SUBTREE_OK)
			return (SYNTAX_ERROR);
	}
	return (SUBTREE_OK);
}

int	is_simple_command(t_token_list **tok, t_parse_tree **new)
{
	t_parse_tree	*cmd_word_node;
	t_parse_tree	*cmd_suffix_node;

	*new = alloc_parse_tree();
	if (*new == NULL)
		return (MEMORY_ERROR);
	if (is_cmd_prefix(tok, &((*new)->child)) != SUBTREE_OK)
		return (free_parse_tree(*new), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
	cmd_word_node = NULL;
	if (is_cmd_word(tok, &cmd_word_node) != SUBTREE_OK)
		return (free_parse_tree(*new), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
	if (cmd_word_node == NULL)// PROBABLY CHECK WHETHER THE TOKEN LIST IS EMPTY
		return (SUBTREE_OK);
	link_node(&((*new)->child), cmd_word_node);
	cmd_suffix_node = NULL;
	if (is_cmd_suffix(tok, &cmd_suffix_node) == SUBTREE_OK)
	{
		if (cmd_suffix_node && cmd_suffix_node->child != NULL)
			link_node(&((*new)->child), cmd_suffix_node);
	}
	else
		return (free_parse_tree(*new), SYNTAX_ERROR);
	return (SUBTREE_OK);
}
