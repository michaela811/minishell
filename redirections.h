/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:33 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/16 14:27:55 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_redirection(t_p_tree **node, t_exec_vars *vars, t_env **env, t_hd_data *here_docs);
void	handle_redirection_from(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars , t_hd_data *here_docs, t_env **env);
char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars, t_env **env);
