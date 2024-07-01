/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:33 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 12:46:32 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_redirection(t_p_tree **node, t_exec_vars *vars, t_env **env);
void	handle_redirection_from(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars);
char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars);
void	is_there_here_doc(t_free_data *exec_data, t_p_tree *tree);