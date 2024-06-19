#include "minishell.h"

int	execute_parse_tree(t_free_data *exec_data)
{
    if (exec_data->tree == NULL)
        return (0);
    if (exec_data->tree->sibling)
        execute_pipeline(exec_data);
	else
		execute_node(exec_data);
	return (g_last_exit_status);
}

int	execute_node(t_free_data *exec_data)
{
	t_exec_vars	*vars;

    vars = malloc(sizeof(t_exec_vars));
    if (!vars)
        {return (printf_global_error(1, 2, "my(s)hell: execute_node malloc error\n"), 1);}
	init_exec_vars(vars);
	if (exec_data->tree == NULL)
		return (0);
	while (exec_data->tree != NULL)
	{
		if (exec_data->tree->data != NULL)
		{
			handle_node_data(&exec_data->tree, vars, &exec_data->env);
            if (vars->args[0] == NULL)
                vars->i = 0;    
			if (vars->error != 0)
				return (g_last_exit_status);
		}
        if (vars->i > vars->capacity - 1)
        {
            expand_exec_vars(vars);
            if (vars->error)
                free_env_array(vars->args);
        }
		exec_data->tree = exec_data->tree->child;
	}
	vars->args[vars->i] = NULL;
	execute_command(vars, exec_data);
    free_env_array(vars->args);
    free(vars);
	return (g_last_exit_status);
}

int	execute_pipeline(t_free_data *exec_data)
{
    int		pipefd[2];
    pid_t	pid;

    if (exec_data->tree == NULL)
        return (0);
    if (exec_data->tree->sibling != NULL)
    {
        if (pipe(pipefd) == -1)
            return (printf_global_error(1, 2, "my(s)hell: pipe\n"), 1);
    }
    pid = fork();
    if (pid == -1)
        return (printf_global_error(1, 2, "my(s)hell: fork\n"), 1);
    else if (pid == 0)
        return (handle_child_process(pipefd, exec_data));
    else
        return (handle_parent_process(pipefd, pid, exec_data));
}
