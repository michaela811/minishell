#include "lexer.c"

void cleanup_and_exit(t_env *env, t_token_list *token_list, int exit_code)
{
    free_env(env);
    free_token_list(token_list);
    exit(exit_code);
}