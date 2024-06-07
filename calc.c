#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_line(FILE *stream);
float calc(char *input);

// Simple calculator made in C by alicela1n, 2024

// *get_line(): Read a line and return it as a string
char *get_line(FILE *stream) {
    const size_t chunk = 4096;
    size_t max_len = chunk;
    char *line = malloc(chunk + 1);
    if (!line)
        return NULL;

    size_t i = 0;
    printf("> ");
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

// calc_prompt(): Prompt for input
void calc_prompt(void) {
    // Sets the size of the character array for the prompt input
    char *line = NULL;

    while ((line = get_line(stdin))) {
    
	// Exit the program if it detects exit being typed in.
	if(strcmp(line, "exit\n")==0||strcmp(line, "quit\n")==0||strcmp(line,"q\n")==0) {
	    exit(0);
	}

	float result = calc(line);
	printf("%f\n", result);
	free(line);
    }
}

// calc(): Do calculations
float calc(char *input) {
    float a, b;
    float result = 0;
    char op;

    for (int i = 0; input[i] != '\0'; i++) {

	sscanf(input, "%g %c %g", &a, &op, &b);

	if (op == '+')
	    result = a + b;
	else if (op == '-')
	    result = a - b;
	else if (op == '/')
	    result = a / b;
	else if (op == '*')
	    result = a * b;
	else
	    printf("ERROR!\n");

	return result;
    }

    return 0;
}

// main(): Main function
int main(void) {
    printf("Use exit, quit, or q to quit.\n");
    calc_prompt();
    return 0;
}
