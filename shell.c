#include "shell.h"

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

