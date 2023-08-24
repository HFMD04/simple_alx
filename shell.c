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

	printf("#myshell$ ");
	if (getline(&user_input, &buffer_size, stdin) == -1)
	{
		if (feof(stdin))
		{
			printf("\n");
			user_input = "";
			return (EXIT_SUCCESS);
		}
		else
		{
			perror("Input reading error");
			user_input = "";
			return (EXIT_FAILURE);
		}
	}
	user_input[strcspn(user_input, "\n")] = '\0';
	args[0] = user_input;
	args[1] = NULL;
	if (execve(user_input, args, NULL) == -1)
	{
		perror("Command execution error");
		user_input = "";
		return (EXIT_FAILURE);
	}
	user_input = "";
}
return (EXIT_SUCCESS);
}
