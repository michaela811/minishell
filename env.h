t_env	*create_env_var(const char *name, const char *value);
t_env	*init_environment(char **envp);
t_env	*find_env_var(t_env *head, const char *name);
int		update_add_env_var(t_env **head, const char *name,
			const char *value);
int		count_env_list(t_env *head);
char	*create_env_str(t_env *current);
void	free_env_array(char **env_array);
char	*get_env_var(t_env *head, const char *name);
char	**env_list_to_array(t_env *head);
void	handle_global_env(t_p_tree **node, char **args, int i, t_env **env);