#include "minishell.h"

int	execute_parse_tree(t_free_data *free_data)
{
    //printf("Entering execute_parse_tree function\n");

    if (free_data->tree == NULL)
        return (0); //NO IMPUT, NOT A MISTAKE
    if (free_data->tree->sibling)
    {
        //printf("Calling execute_pipeline function\n");
        execute_pipeline(free_data);
    }
	else
	{
		execute_node(free_data);
	}
	return (g_last_exit_status);
}

int	execute_node(t_free_data *free_data)
{
	t_exec_vars	vars;

	init_exec_vars(&vars);
	if (free_data->tree == NULL)
		return (0);
	while (free_data->tree != NULL)
	{
		if (free_data->tree->data != NULL)
		{
			handle_node_data(&free_data->tree, &vars, &free_data->env);
			if (vars.error != 0)
				return (g_last_exit_status);
		}
		free_data->tree = free_data->tree->child;
	}
	vars.args[vars.i] = NULL;
	execute_command(&vars, free_data);
	//if (execute_command(&vars, free_data) == 1)
	//{
	//	g_last_exit_status = 154;
	//	return (1);
	//}
	return (g_last_exit_status);
}

int	execute_pipeline(t_free_data *free_data)
{
    int		pipefd[2];
    pid_t	pid;

    //printf("Entering execute_pipeline function\n");

    if (free_data->tree == NULL)
        return (0);
    if (free_data->tree->sibling != NULL)
    {
        //printf("Sibling is not NULL\n");
        if (pipe(pipefd) == -1)
            return (printf_global_error(1, 2, "my(s)hell: pipe\n"), 1);
    }
    pid = fork();
    if (pid == -1)
        return (printf_global_error(1, 2, "my(s)hell: fork\n"), 1);
    else if (pid == 0)
        return (handle_child_process(pipefd, free_data));
    else
    {
        //printf("Calling handle_parent_process function\n");
        return (handle_parent_process(pipefd, pid, free_data));
    }
}
