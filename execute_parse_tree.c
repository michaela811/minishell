#include "lexer.h"

int	execute_parse_tree(t_parse_tree *root, t_env **env)
{
	if (root == NULL)
		return (0); //NO IMPUT, NOT A MISTAKE
	if (root->sibling)
	{
		if (execute_pipeline(root, env))
		{
			g_last_exit_status = 1;
			return (1);
		}
	}
	else
	{
		if (execute_node(root->child, env))
		{
			g_last_exit_status = 1;
			return (1);
		}
	}
	return (0);
}

int	execute_node(t_parse_tree *node, t_env **env)
{
	t_exec_vars	vars;

	init_exec_vars(&vars);
	if (node == NULL)
		return (0);
	while (node != NULL)
	{
		if (node->data != NULL)
		{
			handle_node_data(node, &vars, env);
			if (vars.error == 1)
				return (vars.error);
		}
		node = node->child;
	}
	vars.args[vars.i] = NULL;
	if (execute_command(&vars, env) == 1)
	{
		g_last_exit_status = 154;
		return (1);
	}
	return (0);
}

int	execute_pipeline(t_parse_tree *node, t_env **env)
{
	int		pipefd[2];
	pid_t	pid;

	if (node == NULL)
		return (0);
	if (node->sibling != NULL)
	{
		if (pipe(pipefd) == -1)
		{
			g_last_exit_status = 1;
			return (perror("pipe"), 1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		g_last_exit_status = 1;
		perror("fork");
		return (1);
	}
	else if (pid == 0)
		return (handle_child_process(node, env, pipefd));
	else
		return (handle_parent_process(node, env, pipefd, pid));
}
