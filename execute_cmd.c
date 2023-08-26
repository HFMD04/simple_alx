#include "shell.h"

/**
 * execute_command - Execute a command using execve.
 *
 * @command: The command to execute.
 */
void execute_command(char *command)
{
	char *args[2];

	args[0] = command;
	args[1] = NULL;

	if (execve(command, args, environ) == -1)
	{
		perror("Command execution error");
		free(command);
		exit(EXIT_FAILURE);
	}
	/* Free the command memory before calling execve */
	free(command);
}
