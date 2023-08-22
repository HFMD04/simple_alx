#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LEN 100

int main(void) {
    char *input_str;
    size_t buffer_size = MAX_INPUT_LEN;

    while (1) {
        printf("$ "); // Display the shell prompt

        // Allocate memory for user input
        input_str = (char *)malloc(buffer_size * sizeof(char));
        if (input_str == NULL) {
            perror("Memory allocation error");
            return EXIT_FAILURE;
        }

        // Read user's input
        if (getline(&input_str, &buffer_size, stdin) == -1) {
            if (feof(stdin)) {
                printf("\n");
                free(input_str);
                return EXIT_SUCCESS;
            } else {
                perror("Input reading error");
                free(input_str);
                return EXIT_FAILURE;
            }
        }

        // Remove newline character
        input_str[strcspn(input_str, "\n")] = '\0';

        // Tokenize input for command and arguments
        char *cmd = strtok(input_str, " ");
        char *args[MAX_INPUT_LEN];
        int arg_count = 0;

        while (cmd != NULL) {
            args[arg_count] = cmd;
            arg_count++;
            cmd = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Null-terminate argument array

        // Check if the command is "exit"
        if (strcmp(args[0], "exit") == 0) {
            // Free allocated memory
            free(input_str);
            return EXIT_SUCCESS;
        }

        // Check if the command is "env"
        if (strcmp(args[0], "env") == 0) {
            char **env = environ;
            while (*env != NULL) {
                printf("%s\n", *env);
                env++;
            }
            // Free allocated memory
            free(input_str);
            continue;
        }

        // Check for command existence in PATH and execute if found
        if (access(args[0], F_OK) != -1) {
            // Create child process
            pid_t child_pid = fork();

            if (child_pid == -1) {
                perror("Process creation error");
                free(input_str);
                return EXIT_FAILURE;
            } else if (child_pid == 0) { // Child process
                if (execvp(args[0], args) == -1) {
                    perror("Command execution error");
                    free(input_str);
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
        free(input_str);
    }

    return EXIT_SUCCESS;
}

