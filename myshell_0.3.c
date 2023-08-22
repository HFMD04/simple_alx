#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_CMD_LEN 100

int main(void) {
    char *user_input;
    size_t buffer_size = MAX_CMD_LEN;

    while (1) {
        printf(":) "); // New prompt

        // Allocate memory for user input
        user_input = (char *)malloc(buffer_size * sizeof(char));
        if (user_input == NULL) {
            perror("Memory allocation error");
            return EXIT_FAILURE;
        }

        // Read user's input
        if (getline(&user_input, &buffer_size, stdin) == -1) {
            if (feof(stdin)) {
                printf("\n");
                free(user_input);
                return EXIT_SUCCESS;
            } else {
                perror("Input reading error");
                free(user_input);
                return EXIT_FAILURE;
            }
        }

        // Remove newline character
        user_input[strcspn(user_input, "\n")] = '\0';

        // Tokenize input for command and arguments
        char *command = strtok(user_input, " ");
        char *args[MAX_CMD_LEN];
        int arg_count = 0;

        while (command != NULL) {
            args[arg_count] = command;
            arg_count++;
            command = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Null-terminate argument array

        // Check for command existence in PATH and execute if found
        if (access(args[0], F_OK) != -1) {
            // Create child process
            pid_t child_pid = fork();

            if (child_pid == -1) {
                perror("Process creation error");
                free(user_input);
                return EXIT_FAILURE;
            } else if (child_pid == 0) { // Child process
                if (execvp(args[0], args) == -1) {
                    perror("Command execution error");
                    free(user_input);
                    return EXIT_FAILURE;
                }
            } else { // Parent process
                // Wait for child process to finish
                waitpid(child_pid, NULL, 0);
            }
        } else {
            printf("Command not found: %s\n", args[0]);
        }

        // Free allocated memory
        free(user_input);
    }

    return EXIT_SUCCESS;
}

