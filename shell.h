#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void read_command(char*command, size_t size);
void dislay_prompt(void);
void execute_command(const char*command);
void leo_print(const char*message);

#define MAX_CMD_LEN 100

#endif /* SHELL_H */

