#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if(!file) {
        fprintf(stderr, "Count not open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buff = malloc(length + 1);
    if (!buff) {
        fprintf(stderr, "Memory allocation failure\n");
        fclose(file);
        return NULL;
    }

    fread(buff, 1, length, file);
    buff[length] = '\0';

    fclose(file);
    return buff;
}

int main(int argc, char** argv){

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    char* input = read_file(argv[1]);
    if (!input) {
        return 1;
    }
    // const char* input = "y > x;";
    // const char* input = "var x = 10";
    // const char* input = "var x = 10; var y = x + 5 * 2;";
    // const char* input = "var x = 10; var y = x + 5 * 2; x = y;";
    // const char* input = "print(10);";
    // const char* input = "var x = 10; print(x);";
    // const char* input = "var x = 1; while(x < y) {x = x + 1;}";
    // const char* input = "var x = 10; if (x > 5) then {var y = x + 5 * 2;} print(y);";
    // const char* input = "var x = 10; if (x > 5) then {var y = x + 5 * 2; print(y);}";
    // const char* input = "var x = 10; if (x > 5) then {var y = x + 5 * 2; var z = 25;}";
    // const char* input = "var x = 10; while (x > z) {print(x); var z = 25;}";
    // const char* input = "var x = 10; if (10 > 20) then {var y = x + 5 * 2;}";
    Lexer lexer;
    init_lexer(&lexer, input);
    tokenize(&lexer);
    for(int i = 0; i < lexer.count; i++) {
        print_token(lexer.tokens[i]);
    }

    // free(input); // I think we can get rid of these as soon as we lex?

    Parser parser;
    init_parser(&parser, lexer.tokens, lexer.count);
    ASTNode* program = parse_program(&parser);

    if(program == NULL)
    {
        printf("NULL PROGRAM");
    }

    printf("-----------------------------------------\n");

    print_ast(program, 0);


    printf("\n----------------------------------------- ENV: -------------\n");

    Environment env = { .count = 0 };
    eval(program, &env);

    // Print environment after evaluation
    printf("\nEnvironment: Variables\n");
    for (int i = 0; i < env.count; ++i) {
        printf("%s = %d\n", env.variables[i].name, env.variables[i].value);
    }

    printf("\nEnvironment Functions:\n");
    for (int i = 0; i < env.fun_count; ++i) {
        printf("%s(", env.functions[i].name);
        for(size_t j = 0; j < env.functions[i].param_count - 1 ; j++) {
            printf("%s,", env.functions[i].parameters[j]);
        }
        printf(env.functions[i].parameters[env.functions[i].param_count - 1]);
        printf(");\n");
    }

    

    free_ast(program);
    free_lexer(lexer);
    free(input);
    //free(lexer.tokens);
    
    return 0;
}