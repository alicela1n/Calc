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
    OPERATOR_EXP
};

enum associativity {
    LEFT_ASSOCIATIVITY,
    RIGHT_ASSOCIATIVITY,
};

struct token {
    enum token_type type;
    union data {
        float number;
        enum operator operator;
    } data;
};

// Declare functions to be used
char *get_line(FILE *stream);
float calc(float a, float b, enum operator op);
struct token *tokenize_input(char *input, int *tokens_nb);
float parse_expr(struct token *tokens, int tokens_nb, int min_prec, int *read_rd);

// *get_line(): Read a line and return it as a string
char *get_line(FILE *stream) {
    // For performance reasons memory is being allocated in chunks
    const size_t chunk = 4096; // Size of buffer
    size_t max_len = chunk;
    char *line = malloc(chunk + 1); // Allocate the size of the line buffer
    if (!line)
        return NULL;

    size_t i = 0;

    printf("> "); // Prompt to be displayed to the user
    
    // Get the input from the user and store it in the line string
    for (char ch = fgetc(stream); ch != EOF; ch = fgetc(stream)) {

	// Allocate more space when the limit is reached
        if (i == max_len) {
            max_len += chunk;
            char *const tmp = realloc(line, max_len + 1);
            if (!tmp) {
                free(line);
                return NULL;
            }
            line = tmp;
        }

	// Add the character to the line
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

	
	int tokens_count = 0;
	int read_nb = 0;
	struct token *input = tokenize_input(line, &tokens_count);
	float result = parse_expr(input, tokens_count, -1, &read_nb);
	
	printf("%f\n", result);
	
	free(line);
	free(input);
    }
}

// tokenize_input(): Tokenize the input string and return it as an array
//                   of tokens. For input it takes in a string and pointer
//                   to an integer which will be the number of tokens there
//                   are.
struct token *tokenize_input(char *input, int *tokens_nb) {
    struct token *tokens = malloc(1024 * sizeof(struct token)); // Allocate a token array
    
    while (true) {
	const char *lexeme = strtok(input, " "); // Use space as delimiter
	if (lexeme == NULL) // End of input
	    break;
	// Check if the lexeme is a number, and if so set the type to number and
	// store the number in the token.
	if (isdigit(*lexeme)) {
	    const float number = strtof(lexeme, NULL);
	    tokens[*tokens_nb].type = TOKEN_TYPE_NUMBER;
	    tokens[*tokens_nb].data.number = number;
	    ++*tokens_nb; // Increment the token counter
	} else if (*lexeme == '(') {
	    tokens[*tokens_nb].type = TOKEN_TYPE_LEFT_PAREN;
	    ++*tokens_nb;
	} else if (*lexeme == ')') {
	    tokens[*tokens_nb].type = TOKEN_TYPE_RIGHT_PAREN;
	    ++*tokens_nb;
	} else {
	    // Check if the lexeme is an operator, and if so set the token type to operator
	    // and store the operator in the token.
	    tokens[*tokens_nb].type = TOKEN_TYPE_OPERATOR;
	    // Switch between the different operators
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
		case '^':
		    tokens[*tokens_nb].data.operator = OPERATOR_EXP;
		    ++*tokens_nb;
		    break;
	    }
	}
	input = NULL; // Set the input pointer to null
    }
    return tokens; // Return the token array
}

// calc(): Do calculations
float calc(float left, float right, enum operator op) {
    
    if (op == OPERATOR_ADD)
	return left + right;
    else if (op == OPERATOR_SUB)
	return left - right;
    else if (op == OPERATOR_DIV)
	return left / right;
    else if (op == OPERATOR_MUL)
	return left * right;
    // Error handling
    printf("ERROR!\n");
    exit(1);
}

float compute_atom(struct token *tokens, int tokens_nb, int *read_nb) {
    if (tokens_nb < 1) {
	printf("Expected number, found nothing\n");
	exit(1);
    }

    if (tokens->type == TOKEN_TYPE_NUMBER) {
	++*read_nb;
	return tokens->data.number;
    } else {
	printf("Invalid token\n");
	exit(1);
    }
}

float parse_expr(struct token *tokens, int tokens_nb, int min_prec, int *read_nb) {
    *read_nb = 0;
    float left_atom = compute_atom(tokens, tokens_nb, read_nb);
    tokens += *read_nb;
    tokens_nb -= *read_nb;

    int precedence_table[] = {
        [OPERATOR_ADD] = 0,
	[OPERATOR_SUB] = 0,
	[OPERATOR_MUL] = 1,
	[OPERATOR_DIV] = 1,
	[OPERATOR_EXP] = 2,
    };

    enum associativity associativity_table[] = {
	[OPERATOR_ADD] = LEFT_ASSOCIATIVITY,
	[OPERATOR_SUB] = LEFT_ASSOCIATIVITY,
	[OPERATOR_MUL] = LEFT_ASSOCIATIVITY,
	[OPERATOR_DIV] = LEFT_ASSOCIATIVITY,
	[OPERATOR_EXP] = RIGHT_ASSOCIATIVITY
    };
    
    int prec = precedence_table[tokens->data.operator];

    while (true) {
	if (tokens_nb == 0)
	    break;

	if (tokens->type != TOKEN_TYPE_OPERATOR || prec < min_prec) {
	    break;
	}

	enum operator op = tokens->data.operator;
	int next_min_prec = 0;
	if (associativity_table[tokens->data.operator] == LEFT_ASSOCIATIVITY) {
	    next_min_prec = prec + 1;
	} else {
	    next_min_prec = prec;
	}

	// The operatore has been consumed, increment the counter
	++*read_nb;
	++tokens;
	tokens_nb -= 1;

	int rd;
	float right_atom = parse_expr(tokens, tokens_nb, next_min_prec, &rd);
	tokens += rd;
	tokens_nb -= rd;

	// Add the number of tokens read by the recursive calls
	*read_nb += rd;

	left_atom = calc(left_atom, right_atom, op);
    }

    return left_atom;
}

// main(): Main function
int main(void) {
    printf("Use exit, quit, or q to quit.\n");
    calc_prompt();
    return 0;
}
