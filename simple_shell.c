#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 100

int main(void)
{
char *user_input;
size_t bufsize = MAX_CMD_LEN;

while (1)
{
printf("#customshell$ ");
// Allocate memory for the user input command
user_input = (char *)malloc(buffer_size * sizeof(char));
if (user_input == NULL)
{
perror("Memory allocation error");
return (EXIT_FAILURE);
}

// Read the user's input command
if (getline(&user_input, &buffer_size, stdin) == -1)
{
if (feof(stdin)) 
{
printf("\n");// Print a new line for Ctrl+D
 free(user_input);
                return EXIT_SUCCESS;
            } else {
                perror("Input reading error");
                free(user_input);
                return EXIT_FAILURE;
            }
        }

        // Remove the newline character from the end
        user_input[strcspn(user_input, "\n")] = '\0';

  // Create a child process
        pid_t process_id = fork();

        if (process_id == -1) {
            perror("Process creation error");
            free(user_input);
            return EXIT_FAILURE;
        } else if (process_id == 0) { // Child process
            char *args[] = {user_input, NULL};
            if (execve(user_input, args, environ) == -1) {
                perror("Command execution error");
                free(user_input);
                return EXIT_FAILURE;
            }
        } else { // Parent process
            // Wait for the child process to finish
            waitpid(process_id, NULL, 0);
        }

        // Free the allocated memory
        free(user_input);
    }

    return EXIT_SUCCESS;
}
