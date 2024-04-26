#include "lexer.h"

void free_token(t_token* tok)
{
    if (tok == NULL)
        return;
    if (tok->lexeme == NULL)
        return;
    free(tok->lexeme);
    tok->lexeme = NULL;
    free(tok);
    tok = NULL;
}

void free_parse_tree(t_parse_tree *tree)
{
    if (tree == NULL) return;
    t_parse_tree* sibling;
    t_parse_tree* child;
    child = tree->child;
    sibling = tree->sibling;
    if (sibling != NULL)
        free_parse_tree(sibling);
    if (child != NULL)
        free_parse_tree(child);
    if (tree->data != NULL)
    {
        free_token(tree->data);
        tree->data = NULL;
    }
    if(tree != NULL)
    {
        free(tree);
        tree = NULL;
    }
}
void free_token_list(t_token_list* list)
{
    t_token_list* current = list;
    t_token_list* next;

    while (current != NULL)
    {
        next = current->next;
        if (current->token->lexeme != NULL)
        {
            free(current->token->lexeme);
            current->token->lexeme = NULL; // Prevent double-free
        }
        if (current->token != NULL)
        {
            free(current->token);
            current->token = NULL; // Prevent double-free
        }
        if (current != NULL)
        {
            free(current);
            current = NULL; // Prevent double-free
        }
        current = next;
    }

    list = NULL; // Set the list pointer to NULL after freeing
}

void	free_array(char **array) //We probably have this one already
{
	size_t	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void free_env(t_env *head)
{
    t_env *current = head;
    while (current)
	{
        t_env *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
}

void free_env_array(char **env_array)
{
    int i = 0;
    while (env_array[i] != NULL)
    {
        free(env_array[i]);
        i++;
    }
    free(env_array);
}