/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:33 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/03 19:25:12 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_redirection(t_p_tree **node, t_exec_vars *vars, t_env **env,
			int *here_docs);
void	handle_redirection_from(t_p_tree **node, t_exec_vars *vars,
			t_env **env, char *redirect_copy);
void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_env **env, char *redirect_copy);
void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_env **env, char *redirect_copy);
void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars ,
			int *here_docs, t_env **env);
char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars, t_env **env);
int		process_heredoc_dollar_open(int file, t_exec_vars *vars, t_env **env,
		char *lexeme_no_quotes);
void	process_heredoc_dollar_closed(int file, char *lexeme_no_quotes);
void	remove_quotes(char **lexeme_ptr, int *error);
int		open_heredoc_file(char *filename, t_exec_vars *vars);
int		helper_is_dir(char	*exp_lexeme, t_exec_vars *vars);
void	helper_fd_out_checker(t_p_tree **node, t_exec_vars *vars);
