#include "lexer.h"

void handle_global_env(t_parse_tree *node, char **args, int i, t_env **env)
{
    char *env_var_name = node->data->lexeme + 1;
    char *env_var_value = get_env_var(*env, env_var_name);
    if (env_var_value != NULL)
        args[i] = env_var_value;
    else
        args[i] = "";
}

void handle_quotes_global(t_parse_tree *node, char **args, int i, t_env **env)
{
    char *str = node->data->lexeme + 1;
    str[ft_strlen(str) - 1] = '\0';
    char buffer[1024] = "";
    char *start = str;
    while (1)
    {
        char *dollar = ft_strchr(start, '$');
        if (dollar == NULL)
        {
            ft_strcat(buffer, start);
            break;
        }
        strncat(buffer, start, dollar - start);
        char *var_start = dollar + 1;
        char *var_end = ft_strpbrk(var_start, " \t\n\"'$");
        if (var_end == NULL)
            var_end = var_start + ft_strlen(var_start);
        char var_name[1024];
        ft_strncpy(var_name, var_start, var_end - var_start);
        var_name[var_end - var_start] = '\0';
        char *var_value = get_env_var(*env, var_name);
        if (var_value != NULL)
            ft_strcat(buffer, var_value);
        start = var_end;
    }
    args[i] = ft_strdup(buffer);
}
