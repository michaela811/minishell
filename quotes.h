/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 14:05:40 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		handle_dollar_sign(char **start, t_handle_vars *l_vars,
			t_free_data *exec_data);
void	handle_single_quotes(char **current, char **result, t_exec_vars *vars);
void	handle_double_quotes_split(char **current, t_exec_vars *vars);
void	handle_double_quotes(t_handle_vars *l_vars, t_exec_vars *vars,
			t_free_data *exec_data);
void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars);
int		check_null(void *pointer, int *error_flag);
int		init_handle_vars(t_handle_vars *local_vars, t_exec_vars *vars);
void	handle_quotes_glob(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
void	handle_no_current(t_handle_vars *local_vars, t_exec_vars *vars,
			t_free_data *exec_data, t_p_tree **node);
void	handle_with_current_dollar(t_handle_vars *local_vars, t_exec_vars *vars,
			t_free_data *exec_data, t_p_tree **node);
void	handle_with_current(t_handle_vars *local_vars, t_exec_vars *vars,
			t_free_data *exec_data, t_p_tree **node);
void	handle_no_quotes(t_handle_vars *local_vars, t_exec_vars *vars,
			t_free_data *exec_data, t_p_tree **node);
int		init_handle_quote_redirect(t_handle_vars *local_vars,
			t_p_tree **node);
void	quotes_glob_redirect(t_p_tree **node, t_exec_vars *vars,
			t_free_data *exec_data);
void	handle_no_current_redirect(t_handle_vars *local_vars, t_exec_vars *vars,
			t_free_data *exec_data, t_p_tree **node);
void	handle_with_current_redirect(t_handle_vars *local_vars,
			t_exec_vars *vars, t_free_data *exec_data, t_p_tree **node);
void	handle_no_quotes_redirect(t_handle_vars *local_vars, t_exec_vars *vars,
			t_free_data *exec_data, t_p_tree **node);
int		split_variable(char *arg, int i, t_exec_vars *vars);
int		update_result(char **result, char *updated_result, t_exec_vars *vars);
int		handle_dollar_error(char **token, t_handle_vars *l_vars,
			t_exec_vars *vars, t_free_data *exec_data);
void	handle_error_and_free(t_exec_vars *vars, t_handle_vars *l_vars);
void	handle_error_and_free_redirect(t_exec_vars *vars, t_handle_vars *l_vars,
			t_p_tree **node);
int		buffer_end_space(char *buffer);
void	update_args(t_exec_vars *vars, t_handle_vars *l_vars);
int		remove_quotes(char **lexeme_ptr);
int		resize_buffer(char **buffer, int *buffer_size);
int		ft_strlcat_custom(char *dst, const char *src, int dest_size);
char	*ft_strncat_custom(char *dest, const char *src, size_t n);
int		question_mark(char **start, char *buffer, char *dollar,
			int dollar_status);
void	vars_set_null(t_handle_vars *l_vars, t_exec_vars *vars);