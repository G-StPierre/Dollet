#include "parser.h"
#include<stdio.h>

#include "utility.h"

const char* node_type_to_string(ASTNodeType);

void init_parser(Parser* parser, Token* tokens, size_t count) {
    parser->tokens = tokens;
    parser->count = count;
    parser->current = 0;
}

Token advance(Parser* parser) {
    if (parser->current < parser->count) {
        return parser->tokens[parser->current++];
    }
    Token eof = (Token){.type = TOKEN_EOF};
    return eof;
}

Token peek(Parser* parser){
    if (parser->current < parser->count) {
        return parser->tokens[parser->current];
    }
    Token eof = (Token){.type = TOKEN_EOF};
    return eof;
}

Token peek_next(Parser* parser) {
    if (parser->current + 1< parser->count) {
        return parser->tokens[parser->current + 1];
    }
    Token eof = (Token){.type = TOKEN_EOF};
    return eof;
}

bool match(Parser* parser, TokenType type){
    if(peek(parser).type == type) {
        advance(parser);
        return true;
    }
    return false;
}

Token consume(Parser* parser, TokenType expected) {
    Token token = parser->tokens[parser->current];
    if (token.type != expected) {
        fprintf(stderr, "Parser error: expected token type %d, got %d\n", expected, token.type);
        exit(1); // or handle error
    }
    parser->current++;
    return token;
}

ASTNode* parse_program(Parser* parser) {
    // printf("Parsing program!");
    ASTNode* program = create_empty_program_node();
    if(program == NULL) {
        printf("PROGRAM IS NULL!");
    }

    if(peek(parser).type == TOKEN_EOF) {
        return NULL;
    }

    while(peek(parser).type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(parser);
        if(stmt == NULL || stmt->type >= NODE_TYPE_COUNT) {
            fprintf(stderr, "Skipping unknown or invalid node type: %d\n",
                stmt ? stmt->type : -1);
            break; // or continue, depending on whether this should halt parsing
        }
        if(stmt != NULL) {
            add_statement(program, stmt);
        }
    }
    return program;
}

ASTNode* parse_statement(Parser* parser) {
    Token tok = peek(parser);
    // printf("Parsing statement of type:");
    if (tok.type == TOKEN_VAR) {
        // printf("Parsing statement of type: var_declaration\n");
        return parse_var_decl(parser);
    } else if (tok.type == TOKEN_FUN) {
        // Function declaration!
        // advance(parser); // eat fun
        // printf("TOKEN IS FUNCTION!");
        ASTNode* decl = parse_fun_decl(parser);
        return decl;

    } else if(tok.type == TOKEN_RETURN) {
        advance(parser); // ??? Eat the TOKEN_RETURN
        // printf("\nRETURN TOKEN!\n");
        // fprintf(stdout, "\nTOKEN AFTER RETURN: %d\n", peek(parser).type);
        ASTNode* return_val = parse_expr(parser);
        // Current token is semi colon at this point!!!
        if(peek(parser).type == TOKEN_SEMICOLON) {advance(parser);}
        return create_return_node(return_val);
        // Do I need to eat a semicolon here?
    }else if(tok.type == TOKEN_IDENT && peek_next(parser).type == TOKEN_ASSIGN) {
        // advance(parser);
        ASTNode* ident = parse_identifier(parser);
        advance(parser);
            // Regular identifier
            ASTNode* value = parse_expr(parser);
            return create_assignment_node(ident, value);
    } else if (tok.type == TOKEN_IF) {
        // printf("Parsing statement of type: if statement\n");
        return parse_if_expr(parser);
    } else if (tok.type == TOKEN_IDENT || tok.type == TOKEN_INT) {
        // printf("Parsing statement of type: expression\n");
        if(peek_next(parser).type == TOKEN_LPAREN) {
            // Must be a function call!
            // ASTNode
            ASTNode* fun_call = parse_fun_call(parser);
        } else {
            ASTNode* expr = parse_expr(parser);
            tok = peek(parser);
            return expr;
        }
    if(tok.type == TOKEN_SEMICOLON) {
        advance(parser);
    } else {
        fprintf(stderr, "ERROR: Expected semicolon at the end of the statement. line: %d\n", tok.line);
        return NULL;
    }
        // return parse_expr(parser);
    } else if (tok.type == TOKEN_PRINT) { 
        // Do i want to eat the print token?
        // printf("Found Print Statement!");
        advance(parser); 
        ASTNode* expr = parse_expr(parser);

        if (expr == NULL) {
            fprintf(stderr, "Parse error in print expression. line: %d\n", tok.line); // Not sure if this is right, only makes sense if multi line prints are unavailable!.
            return NULL;
        }
    
        if (!match(parser, TOKEN_SEMICOLON)) {
            fprintf(stderr, "Expected ';' after print statement. line %d\n", tok.line);
            return NULL;
        }

        return create_print_node(expr);
    } else if (tok.type == TOKEN_WHILE) {
        advance(parser);
        
        // !!! This seems to work well!
        if (!match(parser, TOKEN_LPAREN)) {
            fprintf(stderr, "Expected '(' after 'while. line %d\n", tok.line);
            return NULL;
        }
        ASTNode* condition = parse_expr(parser);
        if (!match(parser, TOKEN_RPAREN)) {
            fprintf(stderr, "Expected ')' after while condition. line %d\n", tok.line);
            return NULL;
        }
        // !!! PROBLEM BELOW WITH BODY!
        ASTNode* body = parse_block(parser);
        if (body == NULL) {
            fprintf(stderr, "Failed to parse block in while statement. line %d\n", tok.line);
            return NULL;
        }
        return create_while_node(condition, body);
    }
    return NULL;
}

ASTNode* parse_fun_decl(Parser* parser) {
    // 
    // printf("\nPARSING FUNCTION!\n ");
    advance(parser); // eat fun!
    int capacity = 3; // Obviously this can be a constant somewhere else
    // !! ^^^
    
    ASTNode* name = parse_identifier(parser);
    //printf(name->identifier.name);
    if(!match(parser, TOKEN_LPAREN)) {
        fprintf(stderr, "Parser expected ( after function name. line %d\n", parser->tokens[parser->current].line); // maybe make a function so I don't have to keep repeating this.
        return NULL;
    }
    
    int count = 0;
    ASTNode** params = malloc(sizeof(ASTNode*) * capacity);
    // Go through parameters, separated by TOKEN_COMMA
    while(peek(parser).type != TOKEN_RPAREN) {
        if(peek(parser).type != TOKEN_COMMA) {
            if(count >= capacity) {
                capacity *= 2;
                params = realloc(params, sizeof(ASTNode*) * capacity);
            }
            params[count] = parse_identifier(parser);
            // fprintf(stdout, "Identifier: %s\n", params[count]->identifier.name);
            count++;
        } else {
            advance(parser);
        }
    } 
    if(!match(parser, TOKEN_RPAREN))
    {
        fprintf(stderr, "Parser expected ) after function name. line %d\n", parser->tokens[parser->current].line); // maybe make a function so I don't have to keep repeating this.
        return NULL;
    }
    ASTNode* body = parse_block(parser);

    return create_fun_decl_node(name, params, body, count);
}

ASTNode* parse_fun_call(Parser* parser) {
    ASTNode* fun_callee = parse_identifier(parser);
    int capacity = 3;
    int count = 0;
    ASTNode** arguments = malloc(sizeof(ASTNode*) * capacity);
    while(advance(parser).type != TOKEN_RPAREN) {
        if(peek(parser).type != TOKEN_COMMA) {
            ASTNode* arg = parse_primary(parser);
            if(count >= capacity) {
                capacity *= 2;
                arguments = realloc(arguments, sizeof(ASTNode*) * capacity);
            }
            arguments[count++] = arg;
        }
    }
    return create_fun_call_node(fun_callee, arguments, count);
}

ASTNode* parse_var_decl(Parser* parser) {
    // ASTNode* node;
    if(!match(parser, TOKEN_VAR)) {
        // Handle Error
        fprintf(stderr, "Parser error var_decl: expected token type %d, got %d. line: %d\n", TOKEN_VAR, parser->tokens[parser->current], parser->tokens[parser->current].line);
        return NULL;
    }

    Token ident_token = advance(parser); // Parse the ident?
    
    if(ident_token.type != TOKEN_IDENT) {
        printf("NOT IDENT");
    }

    if(!match(parser, TOKEN_ASSIGN))
    {
        fprintf(stderr, "Parser error var_decl2: expected token type %d, got %d. line: %d\n", TOKEN_EQUAL, parser->tokens[parser->current], parser->tokens[parser->current].line);
        return NULL;
    }

    // !! NOT A FAN OF THIS, CLEAN THIS UP BY USING PARSE_IDENT
    ASTNode* ident = create_identifier_node(strndup(ident_token.start, ident_token.length));

    // Checking if this is ident, or if it's actually a function call!
    if(peek_next(parser).type == TOKEN_LPAREN) {
        // This must be a function call!
        ASTNode* fun_call = parse_fun_call(parser);
        return create_var_decl_node(ident, fun_call);
    }

    ASTNode* expr = parse_expr(parser);
    match(parser, TOKEN_SEMICOLON);

    return create_var_decl_node(ident, expr);
    // /return node;
}

ASTNode* parse_expr(Parser* parser) {
    return parse_comparison(parser);
}

ASTNode* parse_if_expr(Parser* parser) {
    if(!match(parser, TOKEN_IF))
    {
        fprintf(stderr, "Parser error: expected token type %d, got %d. line: %d\n", TOKEN_IF, parser->tokens[parser->current], parser->tokens[parser->current].line);
        return NULL;
    }

    ASTNode* condition = parse_expr(parser);

    if (condition == NULL) {
        fprintf(stderr, "Parser error: invalid condition expression in if-statement. line: %d\n", parser->tokens[parser->current].line);
        return NULL;
    }

    if(!match(parser, TOKEN_THEN)) 
    {
        fprintf(stderr, "Parser error: expected token type %d, got %d. line: %d\n", TOKEN_THEN, parser->tokens[parser->current], parser->tokens[parser->current].line);
        return NULL;
    }

    ASTNode* then_block = parse_block(parser);

    if (then_block == NULL) {
        fprintf(stderr, "Parser error: invalid 'then' branch expression in if-statement. line %d\n", parser->tokens[parser->current].line);
        return NULL;
    }

    return create_if_node(condition, then_block);
}

ASTNode* parse_block(Parser* parser) {
    if (!match(parser, TOKEN_LBRACE)) {
        fprintf(stderr, "Parser error: expected '{'. line: %d\n", peek(parser).line);
        return NULL;
    }

    ASTNode** statements = malloc(sizeof(ASTNode*) * MAX_STATEMENTS);
    size_t statement_count = 0;

    while (parser->current < parser->count &&
           parser->tokens[parser->current].type != TOKEN_RBRACE)
    {
        //fprintf(stdout, "\n Current Token Type: %d\n", peek(parser).type);
        ASTNode* stmt = parse_statement(parser);
        if (stmt) {
            statements[statement_count++] = stmt;
            //fprintf(stdout, "\nStatement found of type: %d\n", stmt->type);
        } else {
            fprintf(stderr, "Parser error: invalid statement in block. line: %d\n", parser->tokens[parser->current].line);
            break;
        }
    }

    if (!match(parser, TOKEN_RBRACE)) {
        fprintf(stderr, "Parser error: expected '}' at end of block. line: %d\n", parser->tokens[parser->current].line);
        return NULL;
    }

    return create_block_node(statements, statement_count);
}

ASTNode* parse_equality(Parser* parser) {
    ASTNode* first_term = parse_comparison(parser);
    Token tok = peek(parser);
    if(tok.type == TOKEN_EQUAL) {
        advance(parser);
        ASTNode* second_term = parse_comparison(parser);
        return create_binary_expr_node(OP_EQ, first_term, second_term);
    } else if (tok.type == TOKEN_NEQ) {
        advance(parser);
        ASTNode* second_term = parse_comparison(parser);
        return create_binary_expr_node(OP_NEQ, first_term, second_term);
    }
    return first_term;
}

ASTNode* parse_comparison(Parser* parser) {

    ASTNode* first_term = parse_term(parser);

    Token tok = peek(parser);

    OperatorType op_type;

    switch (tok.type) {
        case TOKEN_GT:  op_type = OP_GT;  break;
        case TOKEN_GTE: op_type = OP_GTE; break;
        case TOKEN_LT:  op_type = OP_LT;  break;
        case TOKEN_LTE: op_type = OP_LTE; break;
        default:
            return first_term; 
    }

    advance(parser); 

    ASTNode* second_term = parse_term(parser);

    return create_binary_expr_node(op_type, first_term, second_term);
}

ASTNode* parse_term(Parser* parser) {
    ASTNode* left = parse_factor(parser);
    Token tok = peek(parser);

    while (tok.type == TOKEN_PLUS || tok.type == TOKEN_MINUS) {
        advance(parser);
        ASTNode* right = parse_factor(parser);
        OperatorType op = tok.type == TOKEN_PLUS ? OP_ADD : OP_SUB;
        left = create_binary_expr_node(op, left, right);
        tok = peek(parser);
    }

    return left;
}

ASTNode* parse_factor(Parser* parser) {
    ASTNode* left = parse_unary(parser);
    Token tok = peek(parser);

    while (tok.type == TOKEN_MULT || tok.type == TOKEN_DIV) {
        advance(parser);
        ASTNode* right = parse_unary(parser);
        OperatorType op = tok.type == TOKEN_MULT ? OP_MULT : OP_DIV;
        left = create_binary_expr_node(op, left, right);
        tok = peek(parser); 
    }

    return left;
}

ASTNode* parse_unary(Parser* parser){
    if(peek(parser).type == TOKEN_MINUS) {
        advance(parser);
        ASTNode* right = parse_unary(parser);
        return create_unary_expr_node(OP_NEG, right);
    }
        return parse_primary(parser);
}

ASTNode* parse_primary(Parser* parser){
    Token tok = peek(parser);
    ASTNode* node;
    char buffer[tok.length + 1];
    switch(tok.type)
    {
        case TOKEN_LPAREN:
            advance(parser);
            node = parse_expr(parser);
            consume(parser, TOKEN_RPAREN);
            break;
        case TOKEN_INT:
            strncpy(buffer, tok.start, tok.length);
            buffer[tok.length] = '\0';
            int value = atoi(buffer);
            node = create_literal_node(value);
            advance(parser);
            break;
        case TOKEN_IDENT:
            char* name = strndup(tok.start, tok.length); 
            node = create_identifier_node(name);
            advance(parser);
            break;
        default:
            fprintf(stderr, "Unexpected token in primary: %d. line: %d\n", tok.type, parser->tokens[parser->current].line);
            break;
    }
    return node;
}

ASTNode* parse_identifier(Parser* parser) {
    Token tok = advance(parser);
    char* name = strndup(tok.start, tok.length); 
    return create_identifier_node(name);
}   

const char* node_type_to_string(ASTNodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "NODE_PROGRAM";
        case NODE_VAR_DECL: return "NODE_VAR_DECL";
        case NODE_BINARY_EXPR: return "NODE_BINARY_EXPR";
        case NODE_IDENTIFIER: return "NODE_IDENTIFIER";
        case NODE_LITERAL: return "NODE_LITERAL";
        case NODE_IF: return "NODE_IF";
        case NODE_BLOCK: return "NODE_BLOCK";
        case NODE_UNARY: return "NODE_UNARY";
        default: return "UNKNOWN_NODE_TYPE";
    }
}
