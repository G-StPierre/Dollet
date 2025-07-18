#pragma once
#include "ast.h"
#include <stdbool.h>

#define DEFAULT_PARAM_CAPACITY 100
#define DEFAULT_VARIABLE_CAPACITY 100
#define DEFAULT_FUN_CAPACITY 100

typedef struct {
    char* name;
    int value;
}Variable;

typedef struct {
    char* name;
    char* parameters[DEFAULT_PARAM_CAPACITY];
    size_t param_count;
    ASTNode* body;
}Function;

typedef enum {
    VAL_INT,
    VAL_NULL,
}ValueType;

typedef struct {
    ValueType type;
    union {
        int int_val;
        // Other types
    };
}Value;

typedef struct  {
    Variable variables[DEFAULT_VARIABLE_CAPACITY];
    size_t count;
    Function functions[DEFAULT_FUN_CAPACITY];
    size_t fun_count;
}Environment;

int eval(ASTNode*, Environment*);