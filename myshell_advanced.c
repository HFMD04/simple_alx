#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LEN 100

int main(void) {
    char input_str[MAX_INPUT_LEN];
    int arg_count;
    char *args[MAX_INPUT_LEN];

    while (1) {
        printf("$ "); // Display the shell prompt

        // Read user's input
        if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
            printf("\n");
            return EXIT_SUCCESS;
        }

        // Remove newline character
        input_str[strcspn(input_str, "\n")] = '\0';

        // Tokenize input for command and arguments
        arg_count = 0;
        args[arg_count] = strtok(input_str, " ");

        while (args[arg_count] != NULL && arg_count < MAX_INPUT_LEN - 1) {
            arg_count++;
            args[arg_count] = strtok(NULL, " ");
        }

        args[arg_count] = NULL; // Null-terminate argument array

        // Check if the command is "exit"
        if (strcmp(args[0], "exit") == 0) {
            return EXIT_SUCCESS;
        }

        // Check for command existence in PATH and execute if found
        if (access(args[0], F_OK) != -1) {
            // Create child process
            pid_t child_pid = fork();

            if (child_pid == -1) {
                perror("Process creation error");
                return EXIT_FAILURE;
            } else if (child_pid == 0) { // Child process
                if (execvp(args[0], args) == -1) {
                    perror("Command execution error");
                    return EXIT_FAILURE;
                }
            } else { // Parent process
                // Wait for child process to finish
                waitpid(child_pid, NULL, 0);
            }
        } else {
            printf("Command not found: %s\n", args[0]);
        }
    }

    return EXIT_SUCCESS;
}

