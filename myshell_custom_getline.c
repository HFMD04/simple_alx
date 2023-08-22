#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

char *custom_getline(void) {
    static char input_buffer[BUFFER_SIZE];
    static size_t buffer_index = 0;
    static ssize_t characters_in_buffer = 0;
    
    if (characters_in_buffer == 0) {
        characters_in_buffer = read(STDIN_FILENO, input_buffer, BUFFER_SIZE);
        if (characters_in_buffer <= 0) {
            return NULL; // No more input or error
        }
        buffer_index = 0;
    }
    
    char *line = malloc(BUFFER_SIZE * sizeof(char));
    if (line == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    size_t line_index = 0;

    while (buffer_index < characters_in_buffer) {
        char c = input_buffer[buffer_index++];
        
        if (c == '\n') {
            line[line_index] = '\0';
            return line;
        }
        
        line[line_index++] = c;
    }

    characters_in_buffer = 0; // All buffer characters used
    return line;
}

int main(void) {
    while (1) {
        printf("$ ");
        
        char *input_line = custom_getline();
        if (input_line == NULL) {
            printf("\n");
            return EXIT_SUCCESS;
        }
        
        printf("Input: %s\n", input_line);
        free(input_line);
    }

    return EXIT_SUCCESS;
}

