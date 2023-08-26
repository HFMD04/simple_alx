#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>

#define MAX_CMD_LEN 100

extern char **environ;
char **split_string(char *input, const char *delimiter, int *argc);
char *read_user_input(void);
void execute_command(char *command);
#endif /* SHELL_H */
