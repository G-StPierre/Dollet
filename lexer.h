#include "token.h"

typedef struct {
    const char* input;
    Token* tokens;
    size_t capacity;
    size_t count;
    size_t current;
    size_t inputLength;
    size_t line;
} Lexer;

void tokenize();
char peekNext();
void free_lexer();                                     
bool is_alpha(char c);
bool is_numeric(char c);
bool is_alpha_numeric(char c);
