#include "ast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

ASTNode* create_if_node(ASTNode* condition, ASTNode* then_branch) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->if_expr.condition = condition;
    node->if_expr.then_branch = then_branch;
    return node;
}


ASTNode* create_literal_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_LITERAL;
    node->literal.value = value;
    return node;
}

ASTNode* create_binary_expr_node(OperatorType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_EXPR;
    node->binary_expr.op = op;
    node->binary_expr.left = left;
    node->binary_expr.right = right;
    return node;
}

ASTNode* create_assignment_node(ASTNode* identifier, ASTNode* value_expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGNMENT;
    node->assignment.ident = identifier;
    node->assignment.value_expr = value_expr;
    return node;
}

ASTNode* create_var_decl_node(ASTNode* identifier, ASTNode* value_expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR_DECL;
    node->var_decl.ident = identifier;
    node->var_decl.value_expr = value_expr;
    return node;
}

ASTNode* create_program_node(ASTNode** statements, size_t count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->program.statements = statements; 
    node->program.count = count;
    return node; 
}

ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT;
    node->print.expression = expr;
    return node;
}

ASTNode* create_fun_decl_node(ASTNode* name, ASTNode** parameters, ASTNode* body, size_t param_count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_FUN_DECL;
    node->func_decl.name = name;
    node->func_decl.count = param_count;
    node->func_decl.parameters = parameters;
    node->func_decl.body = body;
    return node;
}

ASTNode* create_fun_call_node(ASTNode* callee, ASTNode** arguments, size_t arg_count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_FUN_CALL;
    node->func_call.callee = callee;
    node->func_call.arg_count = arg_count;
    node->func_call.arguments = arguments;
    return node;
}

ASTNode* create_return_node(ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    node->return_node.value = value;
    return node;
}

ASTNode* create_empty_program_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->program.capacity = DEFAULT_CAPACITY;
    node->program.count = 0;
    node->program.statements = malloc(sizeof(ASTNode*) * DEFAULT_CAPACITY);
    return node;
}

ASTNode* create_identifier_node(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->identifier.name =  strdup(name);
    return node;
}

ASTNode* create_block_node(ASTNode** statements, size_t count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;
    node->block.count = count;
    node->block.statements = statements;
    return node;
}

ASTNode* create_unary_expr_node(OperatorType op_type, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_UNARY;
    node->unary_expr.op_type = op_type;
    node->unary_expr.right = right;
    return node;
}

ASTNode* create_while_node(ASTNode* condition, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->while_stmt.condition = condition;
    node->while_stmt.body = body;
    return node;
}

void add_statement(ASTNode* program, ASTNode* statement) {
    if(program->program.count >= program->program.capacity) {
        program->program.capacity *= 2;
        program->program.statements = realloc(program->program.statements, sizeof(ASTNode*) * program->program.capacity);
    }
    program->program.statements[program->program.count++] = statement;
}

void free_ast(ASTNode* node) {
    if(!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for(size_t i = 0; i < node->program.count; i++) {
                free_ast(node->program.statements[i]);
            }
            free(node->program.statements);
            break;
        case NODE_ASSIGNMENT:
            free_ast(node->assignment.value_expr);
            break;
        case NODE_VAR_DECL:
            free_ast(node->assignment.value_expr);
            break;
        case NODE_BINARY_EXPR:
            free_ast(node->binary_expr.left);
            free_ast(node->binary_expr.right);
            break;
        case NODE_LITERAL:
            break;
        case NODE_IDENTIFIER:
            free_ast(node->identifier.name);
            break;
        case NODE_IF:
            free_ast(node->if_expr.condition);
            free_ast(node->if_expr.then_branch);
            break;
        case NODE_BLOCK:
            for(size_t i = 0; i < node->block.count; i++){
                free_ast(node->block.statements[i]);
            }
            free(node->block.statements);
            break;
        case NODE_PRINT:
            free(node->print.expression);
            break;
        case NODE_WHILE:
            free(node->while_stmt.condition);
            free(node->while_stmt.body);
            break;
        case NODE_UNARY:
            free_ast(node->unary_expr.right);
            break;
        case NODE_RETURN:
            free_ast(node->return_node.value);
            break;
        case NODE_FUN_DECL:
            for(size_t i = 0; i < node->func_decl.count; i++) {
                free_ast(node->func_decl.parameters[i]);
            }
            free(node->func_decl.name);
            free(node->func_decl.body);
            return;
    }
    free(node);
}
// void print_ast(ASTNode* node, int indent);

void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (node == NULL) {
        print_indent(indent);
        printf("NULL\n");
        return;
    }

    //  printf("DEBUG: node type = %d\n", node->type);  // Add this line

    // node->type = NODE_PROGRAM;

    if(node->type == (void*)0) {
        printf("NULL NODE TYPE\n");
    }

    switch (node->type) {
        case NODE_PROGRAM:
            print_indent(indent);
            printf("Program:\n");
            for (size_t i = 0; i < node->program.count; ++i) {
                print_ast(node->program.statements[i], indent + 1);
            }
            break;
        case NODE_VAR_DECL:
            print_indent(indent);
            printf("VarDecl: %s\n", node->assignment.ident->identifier.name);
            print_ast(node->assignment.value_expr, indent + 1);
            break;
        case NODE_ASSIGNMENT:
            print_indent(indent);
            printf("Assignment:\n");
            print_indent(indent + 1);
            printf("Left:\n");
            print_ast(node->assignment.ident, indent + 2);
            print_indent(indent + 1);
            printf("Right:\n");
            print_ast(node->assignment.value_expr, indent + 2);
            break;
        case NODE_IF:
            print_indent(indent);
            printf("If:\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            print_ast(node->if_expr.condition, indent + 2);
            print_indent(indent + 1);
            printf("Then:\n");
            print_ast(node->if_expr.then_branch, indent + 2);
            break;
        case NODE_BLOCK:
            print_indent(indent);
            printf("Block:\n");
            for (size_t i = 0; i < node->block.count; ++i) {
                print_ast(node->block.statements[i], indent + 1);
            }
            break;
        case NODE_BINARY_EXPR:
            print_indent(indent);
            printf("BinaryExpr: %d\n", node->binary_expr.op);
            print_ast(node->binary_expr.left, indent + 1);
            print_ast(node->binary_expr.right, indent + 1);
            break;
        case NODE_UNARY:
            print_indent(indent);
            printf("UnaryExpr: %d\n", node->unary_expr.op_type);
            print_ast(node->unary_expr.right, indent + 1);
            break;
        case NODE_LITERAL:
            print_indent(indent);
            printf("IntLiteral: %d\n", node->literal.value);
            break;
        case NODE_IDENTIFIER:
            print_indent(indent);
            printf("Identifier: %s\n", node->identifier.name);
            break;
        case NODE_PRINT:
            print_indent(indent);
            printf("Print Statement \n");
            print_ast(node->print.expression, indent + 1);
            break;
        case NODE_WHILE:
            print_indent(indent);
            printf("While Statement\n");
            printf("\tCondition:\n");
            print_ast(node->while_stmt.condition, indent + 3);
            printf("\tBody:\n");
            print_ast(node->while_stmt.body, indent + 3);   
            break;
        case NODE_FUN_DECL:
            print_indent(indent);
            printf("Function Declaration:\n");
        
            print_indent(indent + 1);
            printf("Name:\n");
            print_ast(node->func_decl.name, indent + 2);  // should be NODE_IDENTIFIER
        
            print_indent(indent + 1);
            printf("Parameters:\n");
            for (size_t i = 0; i < node->func_decl.count; i++) {
                print_ast(node->func_decl.parameters[i], indent + 2);  // also NODE_IDENTIFIER
            }
        
            print_indent(indent + 1);
            printf("Body:\n");
            print_ast(node->func_decl.body, indent + 2);  // likely a NODE_BLOCK
            break;
        case NODE_FUN_CALL:
            print_indent(indent);
            printf("Function Call:\n");
            print_indent(indent + 1);
            printf("Callee:\n");
            print_ast(node->func_call.callee, indent + 2);
        
            if (node->func_call.arg_count > 0) {
                print_indent(indent + 1);
                printf("Arguments:\n");
                for (size_t i = 0; i < node->func_call.arg_count; i++) {
                    print_ast(node->func_call.arguments[i], indent + 2);
                }
            } else {
                print_indent(indent + 1);
                printf("Arguments: (none)\n");
            }
            break;
        case NODE_RETURN:
            print_indent(indent);
            printf("Return Node\n");
            print_ast(node->return_node.value, indent + 1);
        default:
            print_indent(indent);
            // printf("Node Case Math: %d\n", node)
            printf("Unknown node type: %d\n", node->type);
            break;
    }
}
