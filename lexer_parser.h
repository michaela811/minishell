/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:28 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:37:30 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_token_list		*create_token(enum e_token_type type, char *lexeme);
void				add_token(t_token_list **list, enum e_token_type type,
						char *lexeme);
int					count_additional_chars(char *input, const char *delim);
char				*preprocess_input(char *str, const char *delim);
void				process_input_str(char *str, const char *delim, char *dest);
int					lexer(char *input, t_token_list **tokenList);
int					create_and_add_token(char **tokenValue,
						t_token_list **tokenList, t_token_list	**current);
t_token				*create_token_lexer(char *tokenValue);
t_token_list		*create_node_lexer(t_token *newToken);
enum e_token_type	determine_token_type(char *token_value);
void				link_node(t_p_tree **current, t_p_tree *newNode);
int					is_cmd_word(t_token_list **tok,
						t_p_tree **cmd_word_node);
int					is_simple_command(t_token_list **tok, t_p_tree **new);
int					is_io_redirect(t_token_list **tok,
						t_p_tree **redirect_node);
bool				is_redirection_token(enum e_token_type type);
int					is_cmd_prefix(t_token_list **tok,
						t_p_tree **prefix_node);
int					is_cmd_suffix(t_token_list **tok,
						t_p_tree **suffix_node);
void				link_pipe(t_p_tree **current, t_p_tree *newNode);
int					is_pipe_sequence(t_free_data *exec_data);
void				print_p_tree(t_p_tree *tree, int depth);
void				test_parser(void);
void				handle_input(char *input, t_free_data *exec_data);
void				handle_preprocess_input(char *input,
						t_free_data *exec_data);
void				handle_parse_tree(t_free_data *exec_data);
void				print_token_list(t_token_list *list);
