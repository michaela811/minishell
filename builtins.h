/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:30:26 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/17 15:15:58 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			exec_exit(t_exec_vars *vars, t_free_data *exec_data);
int			get_path(char *cmd, t_env *env, char **exec);
int			exec_builtins(t_exec_vars *vars, t_free_data *exec_data);
int			exec_cd(char **args, t_env **env);
int			update_pwd(t_env **env);//, char *cwd);
int			change_directory_and_update(char *path, t_env **env,
				char **args);
				//char *cwd, char **args);
int			exec_echo(t_exec_vars *vars);
char		*handle_quotes_heredoc(char *input, int *error);
//void		process_args(char **args, int *error);
int			exec_pwd(void);//(t_env **env);
//int			exec_dollar_pwd(void);
int			exec_env(char **args, char **environment);
int			exec_unset(char **args, t_env **env);
void		exec_export_no_args(t_env *env);
void		init_free_name_value(char **name, char **value, int i);
int			split_to_name_value(char **args, char **name, char **value, int *i);
int			exec_update_add_env_var(t_env **env, char *name, char *value);
int			var_control(char *command, char *args);
int			split_var(char *var, char **name, char **value);
int			is_equals(char **name, char **value, char *equals);
int			control_name(char *var, char **name, int *i);
int			exec_export(char **args, t_env **env);
//int			export_quotes(char *input, char **output);
int			exec_global_env(t_exec_vars *vars, t_env **env);
void		expand_exec_vars(t_exec_vars *vars);
void		return_exit_status(char *result, int *i, int *j, int *error);