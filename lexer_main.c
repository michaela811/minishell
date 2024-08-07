/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:22 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 17:18:26 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexer(char *input, t_token_list **token_list)
{
	t_token_list	*current;
	char			*token_value;

	*token_list = NULL;
	current = NULL;
	token_value = ft_strtok(input, (char []){-1, 0});
	while (token_value != NULL)
	{
		if (create_and_add_token(&token_value, token_list, &current))
			return (1);
	}
	return (0);
}

int	create_and_add_token(char **token_value, t_token_list **token_list,
t_token_list **current)
{
	t_token			*new_token;
	t_token_list	*new_node;

	new_token = create_token_lexer(*token_value);
	if (new_token == NULL)
		return (free_token_list(*token_list),
			print_err(1, 2, "malloc error in strdup function\n"), 1);
	new_node = create_node_lexer(new_token);
	if (new_node == NULL)
		return (free_token(new_token), free_token_list(*token_list),
			print_err(1, 2, "malloc error in strdup function\n"), 1);
	if (*token_list == NULL)
	{
		*token_list = new_node;
		*current = *token_list;
	}
	else
	{
		(*current)->next = new_node;
		*current = (*current)->next;
	}
	*token_value = ft_strtok(NULL, (char []){-1, 0});
	return (0);
}

t_token	*create_token_lexer(char *token_value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	new_token->type = determine_token_type(token_value);
	new_token->lexeme = ft_strdup(token_value);
	if (new_token->lexeme == NULL)
	{
		free(new_token);
		return (print_err(1, 2, "malloc error 29\n"), NULL);
	}
	return (new_token);
}

t_token_list	*create_node_lexer(t_token *new_token)
{
	t_token_list	*new_node;

	new_node = malloc(sizeof(t_token_list));
	if (new_node == NULL)
	{
		free(new_token);
		return (print_err(1, 2, "malloc error 30\n"), NULL);
	}
	new_node->token = new_token;
	new_node->next = NULL;
	return (new_node);
}

enum e_token_type	determine_token_type(char *token_value)
{
	if (ft_strcmp(token_value, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(token_value, ">") == 0)
		return (RED_TO);
	else if (ft_strcmp(token_value, "<") == 0)
		return (RED_FROM);
	else if (ft_strcmp(token_value, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(token_value, "<<") == 0)
		return (HERE_DOC);
	else
		return (WORD);
}
