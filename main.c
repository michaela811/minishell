#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAXARGS 128

struct command
{
	int argc;
	char *argv[MAXARGS];
	enum builtin_t
	{
		PWD, NONE//, QUIT, JOBS, BG, FG
	} builtin;
};

const int MAXLINE = 1024;
char prompt[] = "lnsh> ";

void eval(char *cmdline);
int parse(const char *cmdline, struct command *cmd);
void runSystemCommand(struct command *cmd, int bg);
enum builtin_t parseBuiltin(struct command *cmd);
void runBuiltinCommand(struct command *cmd, int bg);
void printWorkingDirectory();
void error(const char* msg);

int main(int argc, char **argv)
{
	char	cmdline[MAXLINE];

	while (1)
	{
		printf("%s", prompt);
		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			error("fgets error");
		if (feof(stdin))
		{
			printf("\n");
			exit(0);
		}
		cmdline[strlen(cmdline) - 1] = '\0';
		eval(cmdline);
	}
}

void	eval(char *cmdline)
{
	int		bg;
	struct	command cmd;

	printf("Evaluating '%s'\n", cmdline);
	bg = parse(cmdline, &cmd);
	//printf("Found command %s\n", cmd.argv[0]);
	if(bg == -1)
		return;
	if(cmd.argv[0] == NULL)
		return;
	if(cmd.builtin == NONE)
		runSystemCommand(&cmd, bg);
	else
		runBuiltinCommand(&cmd, bg);
}

int	parse(const char *cmdline, struct command *cmd)
{
	static char array[MAXLINE];
	const char delims[10] = " \t\r\n";
	char *line = array;
	char *token;
	char *endline;
	int is_bg;

	if(cmdline == NULL)
		error("command line is NULL\n");
	(void)strncpy(line, cmdline, MAXLINE);
	endline = line + strlen(line);
	cmd->argc = 0;
	while (line < endline)
	{
		line += strspn(line,delims);
		if(line >= endline)
			break;
		token = line + strcspn(line, delims);
		*token = '\0';
		cmd->argv[cmd->argc++] = line;
		if (cmd->argc >= MAXARGS - 1)
			break;
		line = token + 1;
	}
	cmd->argv[cmd->argc] = NULL;
	if (cmd->argc == 0)
		return (1);
	cmd->builtin = parseBuiltin(cmd);
	if ((is_bg = (*cmd->argv[cmd->argc - 1] == '&')) != 0)
		return is_bg;
	return (0);
}

void runSystemCommand (struct command *cmd, int bg)
{
	pid_t	childPid;
	if((childPid = fork()) < 0)
		error("fork() error");
	else if (childPid == 0)
	{
		if (execvp(cmd->argv[0], cmd->argv) < 0)
		{
			printf("%s: Command not found\n", cmd->argv[0]);
			exit(0);
		}
	}
	else
	{
		if (bg)
			printf("Child in background [%d]\n", childPid);
		else
			wait(&childPid);
	}
}

enum builtin_t parseBuiltin(struct command *cmd)
{
	if (strcmp(cmd->argv[0], "pwd") == 0)
		return PWD;
    /*else if (strcmp(cmd->argv[0], "quit") == 0)
		return QUIT;
	else if (strcmp(cmd->argv[0], "jobs") == 0)
		return JOBS;
	else if (strcmp(cmd->argv[0], "bg") == 0)
		return BG;
	else if (strcmp(cmd->argv[0], "fg") == 0)
		return FG;*/
	else
		return NONE;
}

void runBuiltinCommand(struct command *cmd, int bg)
{
	switch (cmd->builtin)
	{
	/*case QUIT:
		exit(0);
		break;
	case JOBS:
		printf("Displaying background jobs...\n");
		break;
	case BG:
		printf("Pushing job/process to background...\n");
		break;
	case FG:
		printf("Bringing job/process to foreground...\n");
		break;*/
	case PWD:
		printWorkingDirectory();
		break;
	default:
		printf("Unknown built-in command\n");
		break;
	}
}

void printWorkingDirectory()
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("Current working directory: %s\n", cwd);
	else
		perror("getcwd() error");
}
void error(const char* msg) // Added definition for error function
{
    perror(msg);
    exit(EXIT_FAILURE);
}