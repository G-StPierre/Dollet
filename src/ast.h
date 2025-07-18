#pragma once

#include <stddef.h>

static const size_t DEFAULT_CAPACITY = 2;
static const size_t NODE_TYPE_COUNT = 14; // Maybe this isn't ideal ??
static const size_t DEFAULT_PARAM_SIZE = 5; // Maybe too much?

typedef enum {
    NODE_PROGRAM, // 0
    NODE_VAR_DECL, // 1
    NODE_ASSIGNMENT, // 2
    NODE_BINARY_EXPR, // 3
    NODE_LITERAL, // 4
    NODE_IDENTIFIER, // 5
    NODE_IF, // 6
    NODE_BLOCK, // 7 
    NODE_UNARY, // 8
    NODE_PRINT, // 9
    NODE_WHILE, // 10
    NODE_FUN_DECL, // 11
    NODE_FUN_CALL, // 12
    NODE_RETURN, // 13
} ASTNodeType;

typedef enum {
    OP_ADD, //0 
    OP_SUB, // 1
    OP_MULT, // 2
    OP_DIV,
    OP_GT,
    OP_GTE,
    OP_LT,
    OP_LTE,
    OP_EQ,
    OP_NEQ,
    OP_NEG
} OperatorType;

typedef struct ASTNode {
    ASTNodeType type;

    union {
        struct {
            struct ASTNode** statements;
            size_t count;
            size_t capacity;
        } program;
        struct {
            struct ASTNode* name;
            struct ASTNode** parameters;
            struct ASTNode* body;
            size_t count;
        }func_decl;
        struct{
            struct ASTNode* callee;
            struct ASTNode** arguments;
            size_t arg_count;
        }func_call;
        struct{
            struct ASTNode* value;
        }return_node;
        struct {
            struct ASTNode* expression;
        } print;
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        }while_stmt; // Can't call it while, for obvious reasons
        struct {
            OperatorType op;
            struct ASTNode* left;
            struct AstNode* right;
        } binary_expr;
        struct {
            int value;
        } literal;
        struct {
            struct ASTNode* ident;
            struct ASTNode* value_expr;
        } assignment;
        struct {
            struct ASTNode* ident;
            struct ASTNode* value_expr;
        } var_decl;
        struct {
            char* name;
        } identifier;
        struct {
            struct ASTNode** statements;
            size_t count;
        } block;
        struct {
            OperatorType op_type;
            struct ASTNode* right;
        } unary_expr;
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
        } if_expr;
    };
} ASTNode;

ASTNode* create_literal_node(int value);
ASTNode* create_binary_expr_node(OperatorType op, ASTNode* left, ASTNode* right);
ASTNode* create_assignment_node(ASTNode* identifier, ASTNode* value_expr);
ASTNode* create_var_decl_node(ASTNode* identifier, ASTNode* value_expr);
ASTNode* create_fun_decl_node(ASTNode* name, ASTNode** parameters, ASTNode* body, size_t param_count);
ASTNode* create_fun_call_node(ASTNode* callee, ASTNode** arguments, size_t arg_count);
ASTNode* create_block_node(ASTNode** statements, size_t count);
ASTNode* create_return_node(ASTNode* value);
ASTNode* create_identifier_node(char* name);
ASTNode* create_if_node(ASTNode* condition, ASTNode* then_branch);
ASTNode* create_program_node(ASTNode** statements, size_t count);
ASTNode* create_empty_program_node();
ASTNode* create_unary_expr_node(OperatorType op, ASTNode* right);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_while_node(ASTNode* condition, ASTNode* body);
void print_ast(ASTNode* program, int);
void add_statement(ASTNode* program, ASTNode* statement);
void free_ast(ASTNode* node);