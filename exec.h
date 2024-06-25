int			execute_parse_tree(t_free_data *exec_data);
int			get_exec(char **path, int i, char *cmd, char **exec);
int			handle_child_process(int *pipefd, t_free_data *exec_data);
int			handle_sibling_process(int *pipefd, t_free_data *exec_data);
int			handle_parent_process(int *pipefd, pid_t pid,
				t_free_data *exec_data);
int			execute_pipeline(t_free_data *exec_data);
int			handle_child_cmd(t_exec_vars *vars, t_env **env,
				char **environment);
int			handle_fork(t_exec_vars *vars, t_env **env, char **environment);
int			execute_command(t_exec_vars *vars, t_free_data *exec_data);
void		handle_node_data(t_parse_tree **node, t_exec_vars *vars,
				t_env **env);
int			execute_node(t_free_data *exec_data);