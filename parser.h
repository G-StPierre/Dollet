#pragma once

#include "ast.h"
#include "token.h"

static const size_t MAX_STATEMENTS = 15;
/* GRAMMAR:
Program -> Statement* EOF
Statement -> VarDecl | Assignment | IfExpr | Expr
VarDecl -> 'var' Identifier '=' Expr
Assignment -> Identifier '=' Expr
IfExpr -> 'if' Expr 'then' Block
Block -> "{" Statement "}"
Expr -> Equality
Equality -> Comparison ( ( "==" | "<>") Comparison)*
Comparison -> Term ( (">" | ">=" | "<" | "<=") Term) *
Term -> Factor ( ( "+" | "-") Factor)*
Factor -> Unary ( ( "*" | "/" ) Unary)*
Unary -> ("-") Primary
Primary -> IntegerLiteral | Identifier | "(" Expr ")"
*/

typedef struct {
    Token* tokens;
    size_t current;
    size_t count;
} Parser;

void init_parser(Parser* parser, Token* tokens, size_t count);

Token advance(Parser*);
Token peek(Parser*);
bool match(Parser*, TokenType);

// ??? I don't really need all these exposed, only parse_program I believe, same with stuff above ^^^.
ASTNode* parse_program(Parser*);
ASTNode* parse_statement(Parser*);
ASTNode* parse_var_decl(Parser*);
ASTNode* parse_assignment(Parser*);
ASTNode* parse_fun_decl(Parser*);
ASTNode* parse_fun_call(Parser*);
ASTNode* parse_if_expr(Parser*);
ASTNode* parse_block(Parser*);
ASTNode* parse_expr(Parser*);
ASTNode* parse_comparison(Parser*);
ASTNode* parse_equality(Parser*);
ASTNode* parse_term(Parser*);
ASTNode* parse_factor(Parser*);
ASTNode* parse_unary(Parser*);
ASTNode* parse_primary(Parser*);
ASTNode* parse_identifier(Parser*);