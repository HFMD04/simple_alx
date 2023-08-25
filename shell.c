#include "shell.h"

/**
 * main - simple shell
 *
 * Return: success or error message.
 */
int main(void)
{
char *user_input;
size_t buffer_size = MAX_CMD_LEN;

while (1)
{
	char *args[2];
	pid_t process_id;

	printf("#myshell$ ");
	user_input = (char *)malloc(buffer_size * sizeof(char));
	if (user_input == NULL)
	{
		perror("Memory allocation error");
		return (EXIT_FAILURE);
	}
	if (getline(&user_input, &buffer_size, stdin) == -1)
	{
		if (feof(stdin))
		{
			printf("\n");
			free(user_input);
			return (EXIT_SUCCESS);
		}
		else
		{
			perror("Input reading error");
			free(user_input);
			return (EXIT_FAILURE);
		}
	}
	user_input[strcspn(user_input, "\n")] = '\0';

	process_id = fork();
	if (process_id == -1)
	{
		perror("Process creation error");
		free(user_input);
		return (EXIT_FAILURE);
	}
	else if (process_id == 0)
	{
		args[0] = user_input;
		args[1] = NULL;
		if (execve(user_input, args, environ) == -1)
		{
			perror("Command execution error");
			free(user_input);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(process_id, NULL, 0);
	}
	free(user_input);
}
return (EXIT_SUCCESS);
}
