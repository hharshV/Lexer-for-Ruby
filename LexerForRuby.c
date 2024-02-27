#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
    TOKEN_END,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_LITERAL,
    TOKEN_COMMENT,
    TOKEN_NEWLINE
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

Token get_next_token(FILE *file) {
    Token token;
    char c = fgetc(file);

    // Skip whitespace
    while (isspace(c)) {
        c = fgetc(file);
    }

    if (c == EOF) {
        token.type = TOKEN_END;
        token.lexeme[0] = '\0';
    } else if (isalpha(c) || c == '_') {
        // Identifier or keyword
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(file)) != EOF && (isalnum(c) || c == '_')) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        // Check if it's a keyword
        if (strcmp(token.lexeme, "if") == 0 ||
            strcmp(token.lexeme, "else") == 0 ||
            strcmp(token.lexeme, "while") == 0 ||
            strcmp(token.lexeme, "do") == 0 ||
            strcmp(token.lexeme, "until") == 0 ||
            strcmp(token.lexeme, "puts") == 0) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
    } else if (isdigit(c)) {
        // Literal (number)
        token.type = TOKEN_LITERAL;
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(file)) != EOF && isdigit(c)) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
    } else if (c == '+' || c == '-' || c == '*' || c == '/' ||
               c == '=' || c == '<' || c == '>') {
        // Operator
        token.type = TOKEN_OPERATOR;
        int i = 0;
        token.lexeme[i++] = c;
        if ((c = fgetc(file)) != EOF && ((c == '=' && (token.lexeme[0] == '<' || token.lexeme[0] == '>')) ||
            (token.lexeme[0] == '=' && c == '='))) {
            // Compound operators or equality operator
            token.lexeme[i++] = c;
        } else {
            ungetc(c, file); // Put the character back to the input stream
        }
        token.lexeme[i] = '\0';
    } else if (c == '#') {
        // Comment
        token.type = TOKEN_COMMENT;
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(file)) != EOF && c != '\n') {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
    } else if (c == '\n') {
        token.type = TOKEN_NEWLINE;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
    } else {
        // Unknown token
        token.type = TOKEN_END;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
    }
    return token;
}

void print_lexer_table(FILE *file) {
    printf("| %-20s | %-20s |\n", "Token Type", "Lexeme");
    printf("|----------------------|----------------------|\n");
    Token token;
    do {
        token = get_next_token(file);
        const char *output = NULL;
        switch (token.type) {
            case TOKEN_IDENTIFIER:
                output = "IDENTIFIER";
                break;
            case TOKEN_KEYWORD:
                output = "KEYWORD";
                break;
            case TOKEN_OPERATOR:
                output = "OPERATOR";
                break;
            case TOKEN_LITERAL:
                output = "LITERAL";
                break;
            case TOKEN_COMMENT:
                output = "COMMENT";
                break;
            case TOKEN_NEWLINE:
                output = "NEWLINE";
                break;
            default:
                output = "UNKNOWN";
                break;
        }
        printf("| %-20s | %-20s |\n", output, token.lexeme);
    } while (token.type != TOKEN_END);
}

int main() {
    FILE *file;
    file = fopen("C:\\Users\\hv070\\Documents\\Lexer-for-Ruby\\input.txt", "r"); // Open the file for reading
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    print_lexer_table(file);

    fclose(file); // Close the file
    return 0;
}
