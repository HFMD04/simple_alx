#include "shell.h"

/**
 * read_user_input - Read user input from stdin.
 *
 * Return: The user input as a string or NULL on error.
 */
char *read_user_input(void)
{
	char *input = NULL;
	size_t buffer_size = 0;

	write(STDOUT_FILENO, "$ ", 2);
	if (getline(&input, &buffer_size, stdin) == -1)
	{
		if (feof(stdin))
		{
			printf("\n");
			return (NULL);
		}
		else
		{
			perror("Input reading error");
			return (NULL);
		}
	}
	input[strcspn(input, "\n")] = '\0';
	return (input);
}

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
		exit(EXIT_FAILURE);
	}
}

/**
 * main - simple shell
 *
 * Return: success or error message.
 */
int main(void)
{
	char *user_input;
	bool from_pipe = false;

	while (1 && !from_pipe)
	{
		pid_t process_id;

		if (isatty(STDIN_FILENO) == 0)
			from_pipe = true;
		user_input = read_user_input();
		if (user_input == NULL)
		{
			free(user_input);
			return (EXIT_SUCCESS);
		}

		process_id = fork();
		if (process_id == -1)
		{
			perror("Process creation error");
			free(user_input);
			return (EXIT_FAILURE);
		}
		else if (process_id == 0)
		{
			execute_command(user_input);
		}
		else
		{
			waitpid(process_id, NULL, 0);
		}
		free(user_input);
	}
	return (EXIT_SUCCESS);
}

