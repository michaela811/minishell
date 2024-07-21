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
char		*handle_quotes_echo(char *input, int *error);
void		process_args(char **args, int *error);
int			exec_pwd(void);//(t_env **env);
int			exec_dollar_pwd(void);
int			exec_env(char **args, char **environment);
int			exec_unset(char **args, t_env **env);
void		exec_export_no_args(t_env *env);
int			var_control(char *command, char *args);
int			split_var(char *var, char **name, char **value);
int			exec_export(char **args, t_env **env);
int			export_quotes(char *input, char **output);
int			exec_global_env(t_exec_vars *vars, t_env **env);
void		expand_exec_vars(t_exec_vars *vars);
void		return_exit_status(char *result, int *i, int *j, int *error);