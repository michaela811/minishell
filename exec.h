/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:11 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/02 17:52:00 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			execute_parse_tree(t_free_data *exec_data);
int			get_exec(char **path, int i, char *cmd, char **exec);
int			handle_child_process(int *pipefd, t_free_data *exec_data, t_here_doc_data *here_docs);
pid_t		handle_sibling_process(int *pipefd, t_free_data *exec_data, t_here_doc_data *here_docs);
int			handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data,
			t_here_doc_data *here_docs);
int			execute_pipeline(t_free_data *exec_data, t_here_doc_data *here_docs);
int			handle_child_cmd(t_exec_vars *vars, t_env **env,
				char **environment);
int			handle_fork(t_exec_vars *vars, t_env **env, char **environment);
int			execute_command(t_exec_vars *vars, t_free_data *exec_data);
void		handle_node_data(t_p_tree **node, t_exec_vars *vars,
				t_env **env);
int			execute_node(t_free_data *exec_data);
int			err_check_fork(t_exec_vars *vars, t_env **env,
				char *path);