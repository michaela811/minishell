/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:11 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 11:14:15 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			execute_parse_tree(t_free_data *exec_data);
int			get_exec(char **path, int i, char *cmd, char **exec);
int			handle_child_process(int *pipefd, t_free_data *exec_data);
pid_t		handle_sibling_process(int *pipefd, t_free_data *exec_data);
int			handle_parent_process(int *pipefd, pid_t pid,
				t_free_data *exec_data);
int			execute_pipeline(t_free_data *exec_data);
int			handle_child_cmd(t_exec_vars *vars, t_env **env,
				char **environment, t_free_data *exec_data);
int			handle_fork(t_exec_vars *vars, t_env **env, t_free_data *exec_data);
int			execute_command(t_exec_vars *vars, t_free_data *exec_data);
void		handle_node_data(t_p_tree **node, t_exec_vars *vars,
				t_env **env, int *here_docs);
int			execute_node(t_free_data *exec_data);
int			err_check_fork(t_exec_vars *vars, t_env **env,
				char **path);
int			is_there_here_doc(t_p_tree **tree, int *here_docs);
int			pipe_get_heredoc(t_p_tree **node, t_exec_vars *vars, int fdc);
int			pipe_heredoc(t_p_tree **node, t_exec_vars *vars, int *here_docs);
int			open_heredoc_file(char *filename, t_exec_vars *vars);
int			get_cwd(char *cmd, char **exec, char **path);
int			handle_colon (char **pre_path, t_env *env);
int			exec_cwd(char *cmd, char **exec);
int			exec_from_path(char **path, char *cmd, char **exec);