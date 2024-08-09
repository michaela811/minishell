/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:33 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/09 20:50:22 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		handle_redirection(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data, int *here_docs);
int		handle_redirection_from(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
int		handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
int		handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
int		handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars,
			int *here_docs, t_free_data *exec_data);
char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
int		process_heredoc_dollar_open(int file, t_exec_vars *vars,
			t_free_data *exec_data, char *no_quotes_lex);
int		process_heredoc_dollar_closed(int file, char *no_quotes_lex);
int		helper_is_dir(char	*exp_lexeme, t_exec_vars *vars);
int		helper_fd_out_checker(t_p_tree **node, t_exec_vars *vars);
int		is_ambiguous_redirect(t_p_tree **node, t_exec_vars *vars,
			char *saved_lexeme);
void	redirect_to_error(t_exec_vars *vars, t_p_tree **node);
void	redirect_append_error(t_exec_vars *vars);
int		write_and_free_contents(int fd, char *contents);
int		break_pipe_heredoc(char *buffer, char *contents);
char	*get_heredoc_content(char *contents, char *buffer);
int		pipe_heredoc_get_content(char **contents, char *buffer,
			char *buffer_start);
void	free_bufs_contents(char *contents, char *buffer, char *rl_input);
int		readline_check(char *buffer);
