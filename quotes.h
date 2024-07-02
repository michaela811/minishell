/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:38:28 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_quotes_global(t_p_tree **node, char **args, int i,
			t_env **env);
void	handle_quotes_glob_1(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_quotes_glob(char **arg, t_env **env, int *error);
void	handle_dollar_sign(char **start, char *buffer, t_env **env,
			int buffer_size);
void	handle_single_quotes(char **current, char **result, t_exec_vars *vars);
void	handle_double_quotes_split(char **current, t_exec_vars *vars);
void	handle_double_quotes(char **current, char **result, t_exec_vars *vars,
			t_env **env);
void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars);
int		check_null(void *pointer, int *error_flag);
void	init_handle_vars(t_handle_vars *local_vars, t_exec_vars *vars);
void	handle_quotes_glob_1(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_no_current(t_handle_vars *local_vars, t_exec_vars *vars,
			t_env **env, t_p_tree **node);
void	handle_with_current_dollar(t_handle_vars *local_vars, t_exec_vars *vars,
			t_env **env, t_p_tree **node);
void	handle_with_current(t_handle_vars *local_vars, t_exec_vars *vars,
			t_env **env, t_p_tree **node);
void	handle_no_quotes(t_handle_vars *local_vars, t_exec_vars *vars,
			t_env **env, t_p_tree **node);
void	init_handle_quote_redirect(t_handle_vars *local_vars,
			t_p_tree **node);
void	quotes_glob_redirect(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_no_current_redirect(t_handle_vars *local_vars, t_exec_vars *vars,
			t_env **env, t_p_tree **node);
void	handle_with_current_redirect(t_handle_vars *local_vars,
			t_exec_vars *vars, t_env **env, t_p_tree **node);
void	handle_no_quotes_redirect(t_handle_vars *local_vars, t_exec_vars *vars,
			t_env **env, t_p_tree **node);
int		split_variable(char *arg, int i, t_exec_vars *vars);