/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs_and_frees.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:30:16 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 14:27:38 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_p_tree		*alloc_parse_tree(void);
void			free_parse_tree(t_p_tree *tree);
void			free_token(t_token *tok);
void			free_token_list(t_token_list *list);
void			free_command_data(t_free_data *exec_data);
void			free_exit_data(t_free_data *exec_data);
void			print_err(int status, unsigned int fd,
					char *format, ...);
void			free_env(t_env *head);
void			init_exec_vars(t_exec_vars *vars);
t_free_data		*init_command_data(char **envp, t_free_data *exec_data);
void			free_array(char **array);
void			free_handle_vars(t_handle_vars *l_vars);
void			check_capacity(t_exec_vars *vars);
void			print_echo(int status, unsigned int fd, char *format, ...);
void			free_vars(t_exec_vars *vars);
void			cleanup(t_exec_vars *vars);
void			clean_child_cmd(t_exec_vars *vars, t_free_data *exec_data);
