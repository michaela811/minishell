/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:33 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 16:11:27 by dpadenko         ###   ########.fr       */
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
void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars,
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
int		is_ambiguous_redirect(t_p_tree **node, t_exec_vars *vars,
		char *saved_lexeme);
void	redirect_to_error(t_exec_vars *vars, t_p_tree **node);
void	redirect_append_error(t_exec_vars *vars);
