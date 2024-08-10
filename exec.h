/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:35:11 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 14:02:28 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			execute_parse_tree(t_free_data *exec_data);
int			get_exec(char **path_array, int i, char *cmd, char **path);
int			handle_child_process(int *pipefd, t_free_data *exec_data);
pid_t		handle_sibling_process(int *pipefd, t_free_data *exec_data);
int			handle_parent_process(int *pipefd, pid_t pid,
				t_free_data *exec_data);
int			execute_pipeline(t_free_data *exec_data);
int			handle_child_cmd(t_exec_vars *vars, t_env **env,
				char **environment, t_free_data *exec_data);
int			handle_fork(t_exec_vars *vars, t_env **env, t_free_data *exec_data);
int			execute_command(t_exec_vars *vars, t_free_data *exec_data);
int			handle_node_data(t_p_tree **node, t_exec_vars *vars,
				t_free_data *exec_data, int *here_docs);
int			execute_node(t_free_data *exec_data);
int			err_check_fork(t_exec_vars *vars, t_env **env,
				char **path);
int			is_there_here_doc(t_p_tree **tree, int *here_docs,
				t_free_data *exec_data);
int			get_stdin(void);

int			pipe_heredoc(t_p_tree **node, int *here_docs,
				t_free_data *exec_data);
int			pipe_heredoc_dollar_open(char *no_quotes_lex, int fd,
				t_free_data *exec_data);
int			pipe_heredoc_dollar_closed(char *no_quotes_lex, int fd);
int			open_heredoc_file(char *filename);
int			get_cwd(char *cmd, char **exec, char **path);
int			handle_colon(char **pre_path, t_env *env);
int			exec_cwd(char *cmd, char **exec);
int			exec_from_path(char **path_array, char *cmd, char **path);
int			check_hd_p(char *rl_input, t_handle_vars *l_vars,
				char *no_quotes_lex, char *contents);
int			initialize_vars_pipe_hd(char **rl_input, char **contents,
				t_handle_vars *l_vars);