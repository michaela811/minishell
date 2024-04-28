#include "lexer.h"

int	error_message(char *str) // Probably unnecessary
{
	perror(str);
	return (1);
}

void	execve_error(char **s_cmd) //Lets see if we wnt to use it
{
	g_last_exit_status = 138;
	perror("Execve failed");
	ft_putendl_fd("This command was not found: ", 2);
	ft_putendl_fd(s_cmd[0], 2);
	free_array(s_cmd);
	exit(0);
}