#include "../include/parser/parser.h"
#include "../include/io/logger.h"
#include <string.h>
#include <stdio.h>

// Initializes the parser with the given lexer and sets up the initial tokens.
parser_T *init_parser(lexer_T *lexer)
{
    // Check if lexxer is null
    if (lexer == NULL)
    {
        fprintf(stderr, "Lexer is null\n");
        exit(1);
    }

    // Check if lexer->i is greater than the length of the contents
    if (lexer->i > strlen(lexer->contents))
    {
        fprintf(stderr, "Lexer index is greater than the length of the contents\n");
        exit(1);
    }

    parser_T *parser = calloc(1, sizeof(struct PARSE_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->peek_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;
    return parser;
}

// Consumes the current token if it matches the expected type, otherwise throws an error.
void parser_eat(parser_T *parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = parser->peek_token;
        parser->peek_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        fprintf(stderr, "Unexpected token '%s', expected '%s'\n",
                token_type_to_string(parser->current_token->type),
                token_type_to_string(token_type));
        exit(1);
    }
}

// Parses the entire input and returns the root of the AST.
AST_T *parser_parse(parser_T *parser)
{
    AST_COMPOUND_T *root = (AST_COMPOUND_T *)parser_parse_statements(parser);
    return (AST_T *)root;
}
