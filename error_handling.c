#include "minishell.h"

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
}	//const char	*prompt;

	//prompt = "\nmy(s)hell> ";

void printf_global_error(int status, unsigned int fd, char *format, ...)
{
    va_list args, args_copy;

    va_start(args, format);
    va_copy(args_copy, args);  // Make a copy of the original va_list

    g_last_exit_status = status;
    ft_vprintf_fd(fd, format, args_copy);  // Use the copy for printing

    va_end(args_copy);  // Clean">>" asd up the copied va_list
    va_end(args);       // Clean up the original va_list
}