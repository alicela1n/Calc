// Simple calculator made in C by alicela1n, 2024
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum token_type {
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_LEFT_PAREN,
    TOKEN_TYPE_RIGHT_PAREN
};

enum operator {
    OPERATOR_ADD,
    OPERATOR_SUB,
    OPERATOR_MUL,
    OPERATOR_DIV,
};

struct token {
    enum token_type type;
    union data {
        float number;
        enum operator operator;
    } data;
};

char *get_line(FILE *stream);
float calc(float a, float b, char op);
struct token *tokenize_input(char *input, int *tokens_nb);

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

	
	float a, b;
	char op;
	sscanf(line, "%g %c %g", &a, &op, &b);
	float result = calc(a, b, op);
	printf("%f\n", result);
	

	free(line);
    }
}

// tokenize_input(): Tokenize the input string and return it as an array
//                   of tokens. For input it takes in a string and pointer
//                   to an integer which will be the number of tokens there
//                   are.
struct token *tokenize_input(char *input, int *tokens_nb) {
    struct token *tokens = malloc(1024 * sizeof(struct token)); // Allocate a token array
    
    while (true) {
	const char *lexeme = strtok(input, " ");
	if (lexeme == NULL) // End of input
	    break;
	if (isdigit(*lexeme)) {
	    const float number = strtof(lexeme, NULL);
	    tokens[*tokens_nb].type = TOKEN_TYPE_NUMBER;
	    tokens[*tokens_nb].data.number = number;
	    ++*tokens_nb;
	} else if (*lexeme == '(') {
	    tokens[*tokens_nb].type = TOKEN_TYPE_LEFT_PAREN;
	    ++*tokens_nb;
	} else if (*lexeme == ')') {
	    tokens[*tokens_nb].type = TOKEN_TYPE_RIGHT_PAREN;
	    ++*tokens_nb;
	} else {
	    tokens[*tokens_nb].type = TOKEN_TYPE_OPERATOR;
	    switch(*lexeme) {
		case '+':
		    tokens[*tokens_nb].data.operator = OPERATOR_ADD;
		    ++*tokens_nb;
		    break;
		case '-':
		    tokens[*tokens_nb].data.operator = OPERATOR_SUB;
		    ++*tokens_nb;
		    break;
		case '*':
		    tokens[*tokens_nb].data.operator = OPERATOR_MUL;
		    ++*tokens_nb;
		    break;
		case '/':
		    tokens[*tokens_nb].data.operator = OPERATOR_DIV;
		    ++*tokens_nb;
		    break;
	    }
	}
	input = NULL;
    }
    return tokens;
}

// calc(): Do calculations
float calc(float left, float right, char op) {
    float result = 0;
    
    if (op == '+')
	result = left + right;
    else if (op == '-')
	result = left - right;
    else if (op == '/')
	result = left / right;
    else if (op == '*')
	result = left * right;
    else
	printf("ERROR!\n");
    return result;
}

// main(): Main function
int main(void) {
    printf("Use exit, quit, or q to quit.\n");
    calc_prompt();
    return 0;
}
