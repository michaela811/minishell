#include "lexer.h"

t_free_data *init_command_data(char **envp)
{
    t_token_list    *token_list;
    t_parse_tree    *parse_tree;
    char            **environment;

    token_list = NULL;
    parse_tree = NULL;
    environment = NULL;
    t_free_data *free_data = malloc(sizeof(t_free_data));
    if (!free_data)
        return NULL;
    free_data->env = init_environment(envp);
    if (free_data->env == NULL)
	{
		g_last_exit_status = 1;
		printf("init_environment failed");
        free(free_data);
        return (NULL);
	}
    free_data->token_list = token_list;
    free_data->environment = environment;
    free_data->tree = parse_tree;
    return free_data;
}