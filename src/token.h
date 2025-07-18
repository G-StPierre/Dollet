#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    TOKEN_PROGRAM, // 0
    TOKEN_VAR, // 1
    TOKEN_IDENT, // 2
    TOKEN_INT, // 3
    TOKEN_ASSIGN, // 4
    TOKEN_PLUS,  // 5
    TOKEN_MINUS, // 6
    TOKEN_MULT, // 7
    TOKEN_DIV, // 8 
    TOKEN_SEMICOLON, // 9
    TOKEN_COLON, // 10
    TOKEN_DOT, // 11
    TOKEN_COMMA, // 12
    TOKEN_LPAREN, // 13
    TOKEN_RPAREN, // 14
    TOKEN_LBRACE, // 15
    TOKEN_RBRACE, // 16
    TOKEN_EQUAL, // 17
    TOKEN_LT, // 18
    TOKEN_GT, // 19
    TOKEN_LTE, // 20
    TOKEN_GTE, // 21
    TOKEN_NEQ, // 22
    TOKEN_IF, // 23
    TOKEN_THEN, // 24
    TOKEN_WHILE, // 25
    TOKEN_PRINT, // 26
    TOKEN_FUN, // 27
    TOKEN_RETURN, // 28
    TOKEN_EOF, // 29
    TOKEN_UNKNOWN, // 30

} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int line;
    int length;
} Token;

typedef struct {
    const char* keyword;
    TokenType token_type;
} KeywordMapping;

extern size_t keywords_count;
extern KeywordMapping keywords[];

TokenType get_token_from_keywords(const char* key, int length);
Token create_token(TokenType, size_t, const char*, size_t);
void print_token(Token);