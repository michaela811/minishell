void	handle_redirection(t_p_tree **node, t_exec_vars *vars, t_env **env);
void	handle_redirection_from(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_to(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_env **env);
void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars);
char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars);