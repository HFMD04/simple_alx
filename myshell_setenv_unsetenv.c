#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LEN 100

int main(void) {
    char input_str[MAX_INPUT_LEN];
    int arg_count;
    char *args[MAX_INPUT_LEN];
    bool exit_command = false;

    while (!exit_command) {
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
            exit_command = true;
            if (args[1] != NULL) {
                int exit_status = atoi(args[1]);
                exit(exit_status);
            } else {
                exit(EXIT_SUCCESS);
            }
        }
        // Check if the command is "setenv"
        else if (strcmp(args[0], "setenv") == 0) {
            if (arg_count != 3) {
                fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
            } else {
                if (setenv(args[1], args[2], 1) == -1) {
                    perror("setenv error");
                }
            }
        }
        // Check if the command is "unsetenv"
        else if (strcmp(args[0], "unsetenv") == 0) {
            if (arg_count != 2) {
                fprintf(stderr, "Usage: unsetenv VARIABLE\n");
            } else {
                if (unsetenv(args[1]) == -1) {
                    perror("unsetenv error");
                }
            }
        }
        else {
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
    }

    return EXIT_SUCCESS;
}

