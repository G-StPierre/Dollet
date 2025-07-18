#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

size_t get_ident_end(Lexer*);
void eat_whitespace(Lexer*);

void init_lexer(Lexer* lexer, const char* source)
{
    lexer->input = source;
    lexer->inputLength = strlen(source);
    lexer->count = 0;
    lexer->current = 0;
    lexer->capacity = 16;
    lexer->line = 0;
    lexer->tokens = malloc(sizeof(Token)  * lexer->capacity);
}

void free_lexer(Lexer* lexer) {
    free(lexer->tokens);
    lexer->tokens = NULL;
    lexer->count = 0;
    lexer->capacity = 0;
    lexer->line = 0;
}

void add_token(Lexer* lexer, Token token) {
    if (lexer->count >= lexer->capacity) {
        lexer->capacity *= 2;
        lexer->tokens = realloc(lexer->tokens, sizeof(Token) * lexer->capacity);
    }
    lexer->tokens[lexer->count++] = token;
}

void tokenize(Lexer* lexer)
{
    while(lexer->current < lexer->inputLength) {
        eat_whitespace(lexer);
        Token tok;
        switch(lexer->input[lexer->current]) 
        {
            case '+':
                tok = create_token(TOKEN_PLUS, lexer->line, lexer->input + lexer->current, 1);
                add_token(lexer, tok);
                    lexer->current++;
                break;
            case '-':
                tok = create_token(TOKEN_MINUS, lexer->line, lexer->input + lexer->current, 1);
                add_token(lexer, tok);
                    lexer->current++;
                break;
            case '*':
                tok = create_token(TOKEN_MULT, lexer->line, lexer->input + lexer->current, 1);
                add_token(lexer, tok);
                    lexer->current++;
                break;
            case '/':
                tok = create_token(TOKEN_DIV, lexer->line, lexer->input + lexer->current, 1);
                add_token(lexer, tok);
                    lexer->current++;
                break;
            case '>':
                if(peekNext(lexer) == '='){
                    tok = create_token(TOKEN_GTE, lexer->line, lexer->input + lexer->current++, 2);
                    add_token(lexer, tok);
                    lexer->current++;
                    break; // >=
                } else {
                    tok = create_token(TOKEN_GT, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break; // >
                }
            case '<':
                if(peekNext(lexer) == '='){
                    tok = create_token(TOKEN_LTE, lexer->line, lexer->input + lexer->current++, 2);
                    add_token(lexer, tok);
                    lexer->current++;
                    break; // <=
                } else if(peekNext(lexer) == '>'){
                    tok = create_token(TOKEN_NEQ, lexer->line, lexer->input + lexer->current++, 2);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
                }else {
                    tok = create_token(TOKEN_LT, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break; // <
                }
            case '=':
                if(peekNext(lexer) == '=')
                {
                    tok = create_token(TOKEN_EQUAL, lexer->line, lexer->input + lexer->current++, 2);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
                } else {
                    tok = create_token(TOKEN_ASSIGN, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
                }
            case ';':
                    tok = create_token(TOKEN_SEMICOLON, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
            case ',':
                    tok = create_token(TOKEN_COMMA, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
            case ':':
                    tok = create_token(TOKEN_COLON, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
            case '.':
                    tok = create_token(TOKEN_DOT, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
            case '(':
                    tok = create_token(TOKEN_LPAREN, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;

                    break;
            case ')':
                    tok = create_token(TOKEN_RPAREN, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
            case '{':
                    tok = create_token(TOKEN_LBRACE, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;
                    break;
            case '}':
                    tok = create_token(TOKEN_RBRACE, lexer->line, lexer->input + lexer->current, 1);
                    add_token(lexer, tok);
                    lexer->current++;

                    break;
            default:
                if(is_numeric(lexer->input[lexer->current]))
                {
                    size_t start = lexer->current;
                    size_t end = get_ident_end(lexer);
                    tok = create_token(TOKEN_INT, lexer->line, lexer->input + start, end - lexer->current);
                    lexer->current = end;
                    add_token(lexer, tok);
                    break;
                    // Pray this is an int
                    // Need to keep incrementing until we're no longer numeric.
                } else {
                    size_t start = lexer->current;
                    size_t end = get_ident_end(lexer);
                    TokenType type = get_token_from_keywords(lexer->input + start, end - start);
                    tok = create_token(type, lexer->line, lexer->input + start, end - start);
                    lexer->current = end;
                    add_token(lexer, tok);
                    break;
                    // Need to keep going until whitespace?
                    // get_token_from_keywords()
                }
        }
        // lexer->current++;
    }
    Token eof_token = create_token(TOKEN_EOF, lexer->line, '\0', 0);
    // eof_token.type = TOKEN_EOF;
    // eof_token.start = lexer->current;
    // eof_token.length = 0;
    add_token(lexer, eof_token);
}

char peekNext(Lexer* lexer)
{
    return lexer->input[lexer->current + 1];
}

int is_delimiter(char c) {
    return c == ';' || c == ',' || c == '=' || c == '(' || c == ')'; // Add other punctuation as needed
}


size_t get_ident_end(Lexer* lexer) {
    // I don't like the name get_ident if we're just returning the index of the last letter!
    size_t start = lexer->current;
    while(start <= lexer->inputLength && !isspace(lexer->input[start]) && !is_delimiter(lexer->input[start])) {
        start++;
    }
    return start;
}

void eat_whitespace(Lexer* lexer) {
    while(isspace(lexer->input[lexer->current]))
    {
        if(lexer->input[lexer->current] == '\n') {
            lexer->line++;
        }
        lexer->current++;
    }
}

bool is_alpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool is_numeric(char c) {
    return '0' <= c && '9' >= c;
}

bool is_alpha_numeric(char c) {
    return is_alpha(c) && is_numeric(c);
}

// int main(){
//     const char* input = "var x = 10; var y = x + 5; if x > 5 { var z = y * 2; }";
//     Lexer lexer;
//     init_lexer(&lexer, input);
//     tokenize(&lexer);
//     for(int i = 0; i < lexer.count; i++) {
//         print_token(lexer.tokens[i]);
//     }
    
//     return 0;
// }