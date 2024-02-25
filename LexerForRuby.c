#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
    TOKEN_EOF,
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

Token get_next_token(const char **input) {
    Token token;
    const char *ptr = *input;
    char c = *ptr;

    // Skip whitespace
    while (isspace(c)) {
        c = *(++ptr);
    }

    if (c == '\0') {
        token.type = TOKEN_EOF;
        token.lexeme[0] = '\0';
    } else if (isalpha(c) || c == '_') {
        // Identifier or keyword
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = *(++ptr)) != '\0' && (isalnum(c) || c == '_')) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        *input = ptr; // Update pointer position
        // Check if it's a keyword
        if (strcmp(token.lexeme, "if") == 0 ||
            strcmp(token.lexeme, "else") == 0 ||
            strcmp(token.lexeme, "while") == 0 ||
            strcmp(token.lexeme, "do") == 0 ||
            strcmp(token.lexeme, "until") == 0||
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
        while ((c = *(++ptr)) != '\0' && isdigit(c)) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        *input = ptr; // Update pointer position
    } else if (c == '+' || c == '-' || c == '*' || c == '/' ||
               c == '=' || c == '<' || c == '>') {
        // Operator
        token.type = TOKEN_OPERATOR;
        int i = 0;
        token.lexeme[i++] = c;
        c = *(++ptr);
        if ((c == '=' && (token.lexeme[0] == '<' || token.lexeme[0] == '>')) ||
            (token.lexeme[0] == '=' && c == '=')) {
            // Compound operators or equality operator
            token.lexeme[i++] = c;
            *input = ptr + 1; // Update pointer position
        } else {
            *input = ptr; // Update pointer position
        }
        token.lexeme[i] = '\0';
    } else if (c == '#') {
        // Comment
        token.type = TOKEN_COMMENT;
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = *(++ptr)) != '\0' && c != '\n') {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        *input = ptr; // Update pointer position
    } else if (c == '\n') {
        token.type = TOKEN_NEWLINE;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        *input = ptr + 1; // Update pointer position
    } else {
        // Unknown token
        token.type = TOKEN_EOF;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        *input = ptr + 1; // Update pointer position
    }
    return token;
}

void print_lexer_table(const char *input) {
    printf("| %-20s | %-20s |\n", "Token Type", "Lexeme");
    printf("|----------------------|----------------------|\n");
    Token token;
    do {
        token = get_next_token(&input);
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
    } while (token.type != TOKEN_EOF);
}

int main() {
    const char *input = "num = 0\n"
                        "until num == 10 do\n"
                        "  puts num += 1\n"
                        "end\n";
    print_lexer_table(input);
    return 0;
}
