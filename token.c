#include "token.h"

void print_token(Token token) {
    printf("Token type: %d, value: ", token.type);
    for (int i = 0; i < token.length; i++) {
        putchar(token.start[i]);
    }
    putchar('\n');
}

KeywordMapping keywords[] = {
    {"if", TOKEN_IF},
    {"var", TOKEN_VAR},
    {"program", TOKEN_PROGRAM},
    {"then", TOKEN_THEN},
    {"while", TOKEN_WHILE},
    {"print", TOKEN_PRINT},
    {"fun", TOKEN_FUN},
    {"return", TOKEN_RETURN},
};

size_t keywords_count = sizeof(keywords) / sizeof(keywords[0]);

TokenType get_token_from_keywords(const char* key, int length) {
    char keyword[length + 1];
    strncpy(keyword, key, length);
    keyword[length] = '\0';

    for (size_t i = 0; i < keywords_count; i++) {
        if (strcmp(keywords[i].keyword, keyword) == 0) {
            return keywords[i].token_type;
        }
    }
    return TOKEN_IDENT;
}

Token create_token(TokenType type, size_t line, const char* value, size_t length)
{
    Token token = (Token){.type = type, .line = line, .start = value, .length = length};
    return token;
}