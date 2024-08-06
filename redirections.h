/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:33 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 12:18:46 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_redirection(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data, int *here_docs);
void	handle_redirection_from(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars ,
			int *here_docs, t_free_data *exec_data);
char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
int		process_heredoc_dollar_open(int file, t_exec_vars *vars,
		t_free_data *exec_data, char *lexeme_no_quotes);
int		process_heredoc_dollar_closed(int file, char *lexeme_no_quotes);
void	remove_quotes(char **lexeme_ptr, int *error);
int		open_heredoc_file(char *filename, t_exec_vars *vars);
int		helper_is_dir(char	*exp_lexeme, t_exec_vars *vars);
void	helper_fd_out_checker(t_p_tree **node, t_exec_vars *vars);
