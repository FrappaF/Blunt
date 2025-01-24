#ifndef PARSE_H
#define PARSE_H

#include "../lexer/lexer.h"
#include "../ast/AST.h"

// Structure representing the parser.
typedef struct PARSE_STRUCT
{
    lexer_T *lexer;
    token_T *current_token;
    token_T *peek_token;
    token_T *prev_token;
} parser_T;

/**
 * Initializes the parser with the given lexer.
 * @param lexer The lexer to use for tokenizing the input.
 * @return A pointer to the initialized parser.
 */
parser_T *init_parser(lexer_T *lexer);

/**
 * Consumes the current token if it matches the expected type.
 * @param parser The parser instance.
 * @param token_type The expected token type.
 */
void parser_eat(parser_T *parser, int token_type);

/**
 * Parses the entire input and returns the root of the AST.
 * @param parser The parser instance.
 * @return The root of the AST.
 */
AST_T *parser_parse(parser_T *parser);

#include "parser_statements.h"
#include "parser_expressions.h"

#endif // PARSE_H