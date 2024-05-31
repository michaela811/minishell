#include "minishell.h"

t_free_data *init_command_data(char **envp)
{
    t_free_data *exec_data = malloc(sizeof(t_free_data));
    if (!exec_data)
        return NULL;
    exec_data->env = init_environment(envp);
    if (exec_data->env == NULL)
	{
		g_last_exit_status = 1;
		printf("init_environment failed");
        free(exec_data);
        return (NULL);
	}
    exec_data->token_list = NULL;
    exec_data->environment = NULL;
    exec_data->tree = NULL;
    exec_data->token_list_start = NULL;
    exec_data->tree_start = NULL;
    return exec_data;
}