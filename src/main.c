#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/logger.h"
#include "include/io.h"
#include "include/AST.h"
#include <stdio.h>
#include <string.h>

void print_help()
{
    printf("Usage: blunt <filename> [-v]\n");
}

int main(int argc, char *argv[])
{

    int DO_LEXER = 0;

    if (argc < 2)
    {
        print_help();
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            LOGGING_ENABLED = 1;
        }
        if (strcmp(argv[i], "-l") == 0)
        {
            DO_LEXER = 1;
        }
    }

    lexer_T *lexer = init_lexer(read_file(argv[1]));

    if (DO_LEXER)
    {
        while (lexer->i < strlen(lexer->contents))
        {
            token_T *token = lexer_get_next_token(lexer);
            LOG_PRINT("TOKEN(%s, %s)\n", token_type_to_string(token->type), token->value);
        }
        return 0;
    }

    LOG_PRINT("\nSTARTING PARSER\n");
    parser_T *parser = init_parser(lexer);
    AST_T *root = parser_parse(parser);

    LOG_PRINT("\n ----- PARSER TREE -----\n");
    ast_print(root, 0);
    LOG_PRINT("\n -----------------------\n");

    LOG_PRINT("\nSTARTING VISITOR\n");
    visitor_T *visitor = init_visitor();
    visitor_visit(visitor, root);

    return 0;
}