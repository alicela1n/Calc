#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple calculator made in C by alicela1n, 2024

// Function to read a line
char *get_line(FILE *stream) {
    const size_t chunk = 4096;
    size_t max_len = chunk;
    char *line = malloc(chunk + 1);
    if (!line)
        return NULL;

    size_t i = 0;
    for (char ch = fgetc(stream); ch != EOF; ch = fgetc(stream)) {
        if (i == max_len) {
            max_len += chunk;
            char *const tmp = realloc(line, max_len + 1);
            if (!tmp) {
                free(line);
                return NULL;
            }
            line = tmp;
        }
        line[i] = ch;
        i++;
        if (ch == '\n')
            break;
    }
    line[i] = '\0';
    if (!line[0]) {
        free(line);
        return NULL;
    }

    return line;
}

int calc(void) {
    float a, b;
    float result = 0;
    char op;

    char *line = NULL;
    while ((line = get_line(stdin))) {
        printf("> ");
        if(strcmp(line,"exit\n")==0||strcmp(line,"quit\n")==0||strcmp(line,"q\n")==0) {
            exit(0);
            return 0;
        }
        sscanf(line, "%g %c %g", &a, &op, &b);
        free(line);
        if (op == '+')
            result = a + b; // Addition
        else if (op == '-')
            result = a - b; // Subtraction
        else if (op == '/')
            result = a / b; // Division
        else if (op == '*')
            result = a * b; // Multiplication
        else
            printf("Error!\n");
        printf("%g\n", result);
    }
    return 0;
}

int main(void) {
    printf("Use exit, quit, or q to quit.\n");
    calc();
    return 0;
}
