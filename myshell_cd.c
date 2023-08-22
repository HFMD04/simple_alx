#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_INPUT_LEN 100

int main(void) {
    char input_str[MAX_INPUT_LEN];
    int arg_count;
    char *args[MAX_INPUT_LEN];
    char prev_dir[PATH_MAX];
    char curr_dir[PATH_MAX];
    bool exit_command = false;

    // Initialize prev_dir to home directory
    if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {
        perror("getcwd error");
        return EXIT_FAILURE;
    }

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
        // Check if the command is "cd"
        else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL || strcmp(args[1], "-") == 0) {
                if (chdir(prev_dir) == -1) {
                    perror("chdir error");
                }
            } else {
                // Save current directory in prev_dir
                if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {
                    perror("getcwd error");
                    return EXIT_FAILURE;
                }

                if (chdir(args[1]) == -1) {
                    perror("chdir error");
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

