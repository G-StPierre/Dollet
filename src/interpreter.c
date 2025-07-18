#include "interpreter.h"
#include "utility.h"
#include<stdio.h>

void create_child_environment(Environment*);
Function lookup_function(Environment*, char*, bool*);

int eval(ASTNode* node, Environment* env) {
    switch(node->type) {
        case NODE_LITERAL:
            return node->literal.value;
            break;
        case NODE_IDENTIFIER: {
            bool found;
            int value = lookup_var(env, node->identifier.name, &found);
            if(!found) {
                //printf("Identifier not found!");
            }
            return value;
            break;
        }
        case NODE_WHILE:
            while(eval(node->while_stmt.condition, env)) {
                eval(node->while_stmt.body, env);
            }
            break;
        case NODE_PRINT:
            int print_result = eval(node->print.expression, env);
            printf("%d\n", print_result);
            return print_result;
        case NODE_BINARY_EXPR: {
            int left_val = eval(node->binary_expr.left, env);
            int right_val = eval(node->binary_expr.right, env);
            switch(node->binary_expr.op) {
                case OP_ADD:
                    return left_val + right_val;
                case OP_SUB:
                    return left_val - right_val;
                case OP_DIV:
                    return left_val / right_val;
                case OP_MULT:
                    return left_val * right_val;
                case OP_EQ:
                    return left_val == right_val;
                case OP_NEQ:
                    return left_val != right_val;
                case OP_LT:
                    return left_val < right_val;
                case OP_LTE:
                    return left_val <= right_val;
                case OP_GT:
                    return left_val > right_val;
                case OP_GTE:
                    return left_val >= right_val;
                

            }
            break;
        }
        case NODE_BLOCK:
            for(int i = 0; i < node->block.count; i++) {
                eval(node->block.statements[i], env);
            }
            break;
        case NODE_IF:
            bool condition_met = eval(node->if_expr.condition, env);
            if(condition_met) {
                eval(node->if_expr.then_branch, env);
            }
            break;
        case NODE_UNARY:
            int result = eval(node->unary_expr.right, env);
            if(node->unary_expr.op_type == OP_NEG) {
                return -result;
            }
            return result;
            break;
        case NODE_VAR_DECL:
            int value = eval(node->assignment.value_expr, env);
            return create_var(env, node->assignment.ident, value);
            break;
        case NODE_ASSIGNMENT:
            int val = eval(node->assignment.value_expr, env);
            return create_var(env, node->assignment.ident, val);
            break;
        case NODE_PROGRAM:
            for(int i = 0; i < node->program.count; i++) {
                eval(node->program.statements[i], env);
            }
            break;
        case NODE_FUN_DECL:
            create_func(env, node->func_decl.name, node->func_decl.parameters, node->func_decl.count, node->func_decl.body);
            break; // !!! Think about this?
        case NODE_FUN_CALL:
            bool found_func;
            bool test_bool;
            Function found_function = lookup_function(env, node->func_call.callee->identifier.name, &found_func);
            Value args[100];
            Environment func_env; // Keep scope to function.
            create_child_environment(&func_env);
            for(size_t i = 0; i < node->func_call.arg_count; i++) {
                args[i] = (Value) {.type = VAL_INT, .int_val = eval(node->func_call.arguments[i], env)};
                int t = declare_var(&func_env, found_function.parameters[i], args[i].int_val); // Obviously won't always be int in the future!
                printf("t = %d", t);
                // int x = lookup_var(&func_env, strdup("x\0"), test_bool);
                int x = lookup_var(&func_env, "x\0", &test_bool);
                // printf("x = %d", x);
                // printf("x = %d", x);
                // printf("Search for var: %d\n", lookup_var(&func_env, found_function.parameters[i], NULL));
            }
            return eval(found_function.body, &func_env);
            break;
        case NODE_RETURN:
            return eval(node->return_node.value, env);
        default:
            fprintf(stdout, "Unknown node type: %d\n", node->type);
            return 0;

    }
}

void create_func(Environment* env, ASTNode* ident, ASTNode** params, size_t param_count, ASTNode* body) {
    /// !! Do something here to see if it's already found?
    char* ident_name = strdup(ident->identifier.name);
    Function func = {.name = ident_name, .param_count = param_count, .body = body};
    for(size_t i = 0; i < param_count; i++) {
        // GET PARAMS
        char* param_name = strdup(params[i]->identifier.name);
        func.parameters[i] = param_name;
    }
    env->functions[env->fun_count++] = func;
    return;
}

int declare_var(Environment* env, char* ident, int value){
    bool found;
    int initialized = lookup_var(env, ident, &found);
    if (initialized == -1 && !found) {
        if(env->count > DEFAULT_VARIABLE_CAPACITY) {
            printf("MAX variable limit reached!");
            return -1;
        }
        env->variables[env->count++] = (Variable){.name = ident, .value = value};
        printf("\nIdent: %s, value: %d\n", ident, value);
    } else {
        char* ident_name = strdup(ident);
        update_var(env, ident_name, value);
    }
    return value;
}

int create_var(Environment* env, ASTNode* ident, int value) {
    bool found;
    int initialized = lookup_var(env, ident->identifier.name, &found);
    if (initialized == -1 && !found) {
        if(env->count > DEFAULT_VARIABLE_CAPACITY) {
            printf("MAX variable limit reached!");
            return -1;
        }
        char* ident_name = strdup(ident->identifier.name);
        printf(ident_name);
        env->variables[env->count++] = (Variable){.name = ident_name, .value = value};
        // printf("still fine?");
    } else {
        char* ident_name = strdup(ident->identifier.name);
        update_var(env, ident_name, value);
    }
    return value; // Is this what I want to return?
}

int update_var(Environment* env, char* ident, int value) {
    for(int i = 0; i < env->count; i++) {
        if(strcmp(env->variables[i].name, ident) == 0) {
            env->variables[i].value = value;
            return value;
        }
    }
    return -1; // Not ideal, fix later!!!
}

int lookup_var(Environment* env, char* identifier, bool* found) {
    printf("\nLOOKUP %s\n", identifier);
    printf("ENV COUNT: %d", env->count);
    for(int i = 0; i < env->count; i++) {
        printf("Variable Name %s", env->variables[i].name);
        printf("Name strlen: %zu, Identifier strlen: %zu\n", strlen(env->variables[i].name), strlen(identifier));

        if (env->variables[i].name == NULL) {
            printf("env->variables[%d].name is NULL!\n", i);
            continue;
        }
        if (identifier == NULL) {
            printf("identifier is NULL!\n");
            continue;
        }

        printf("Identifier bytes: \n \n");
        for (int i = 0; identifier[i] != '\0' && i < 32; i++) {
            printf("[%c:%d] ", identifier[i], identifier[i]);
        }
        printf("\n");

        printf("Variable name bytes: ");
        for (int i = 0; env->variables[i].name[i] != '\0' && i < 32; i++) {
            printf("[%c:%d] ", env->variables[i].name[i], env->variables[i].name[i]);
        }
        printf("\n");


        if(strcmp(env->variables[i].name, identifier) == 0) {
            if (found != NULL) {
                *found = true;
            }
            printf("FOUND IDENTIFIER: %s", identifier);
            return env->variables[i].value;
        } else {
            printf("VAR Missing!");
        }
    }
    return -1;
}

Function lookup_function(Environment* env, char* identifier, bool* found) {
    for(size_t i; i < env->fun_count; i++) {
        if(strcmp(env->functions[i].name, identifier) == 0) {
            *found = true;
            return env->functions[i];
        }
    }
}

void create_child_environment(Environment* env) {
    printf("Creating child environemnt");
    env->count = 0;
    env->fun_count = 0;
}