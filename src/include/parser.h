#ifndef PARSE_H
#define PARSE_H

#include "lexer.h"
#include "AST.h"

// Structure representing the parser.
typedef struct PARSE_STRUCT
/**
 * @brief Structure representing a parser.
 *
 * This structure is used to manage the state of the parser, including the lexer and tokens.
 *
 * @var parser_T::lexer
 * Pointer to the lexer instance used for tokenizing the input.
 *
 * @var parser_T::current_token
 * Pointer to the current token being processed by the parser.
 *
 * @var parser_T::peek_token
 * Pointer to the next token to be processed, allowing the parser to look ahead.
 *
 * @var parser_T::prev_token
 * Pointer to the previous token that was processed, useful for backtracking.
 */
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

/**
 * Parses a single statement based on the current token type.
 * @param parser The parser instance.
 * @return The parsed AST node representing the statement.
 */
AST_T *parser_parse_statement(parser_T *parser);

/**
 * Parses a series of statements and returns them as a compound AST node.
 * @param parser The parser instance.
 * @return The parsed compound AST node.
 */
AST_T *parser_parse_statements(parser_T *parser);

/**
 * Parses an expression based on the current token type.
 * @param parser The parser instance.
 * @return The parsed AST node representing the expression.
 */
AST_T *parser_parse_expression(parser_T *parser);

/**
 * Parses a factor (currently unimplemented).
 * @param parser The parser instance.
 * @return The parsed AST node representing the factor.
 */
AST_T *parser_parse_factor(parser_T *parser);

/**
 * Parses a variable definition statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the variable definition.
 */
AST_T *parser_parse_variable_definition(parser_T *parser);

/**
 * Parses a function call.
 * @param parser The parser instance.
 * @return The parsed AST node representing the function call.
 */
AST_T *parser_parse_function_call(parser_T *parser);

/**
 * Parses a function definition.
 * @param parser The parser instance.
 * @return The parsed AST node representing the function definition.
 */
AST_T *parser_parse_function_definition(parser_T *parser);

/**
 * Parses a return statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the return statement.
 */
AST_T *parser_parse_return(parser_T *parser);

/**
 * Parses a string literal.
 * @param parser The parser instance.
 * @return The parsed AST node representing the string literal.
 */
AST_T *parser_parse_string(parser_T *parser);

/**
 * Parses a variable or function call.
 * @param parser The parser instance.
 * @return The parsed AST node representing the variable or function call.
 */
AST_T *parser_parse_variable(parser_T *parser);

/**
 * Parses a dot expression.
 * @param parser The parser instance.
 * @return The parsed AST node representing the nested expression.
 */
AST_T *parser_parse_dot_expression(parser_T *parser);

/**
 * Parses an identifier, which could be a variable or a function call.
 * @param parser The parser instance.
 * @return The parsed AST node representing the identifier.
 */
AST_T *parser_parse_id(parser_T *parser);

/**
 * Parses a variable count statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the variable count.
 */
AST_T *parser_parse_variable_count(parser_T *parser);

/**
 * Parses a variable assignment statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the variable assignment.
 */
AST_T *parser_parse_variable_assignment(parser_T *parser);

/**
 * Parses an integer literal.
 * @param parser The parser instance.
 * @return The parsed AST node representing the integer literal.
 */
AST_T *parser_parse_int(parser_T *parser);

/**
 * Parses an array.
 * @param parser The parser instance.
 * @return The parsed AST node representing the array.
 */
AST_T *parser_parse_array(parser_T *parser);

/**
 * Parses a function argument.
 * @param parser The parser instance.
 * @return The parsed AST node representing the function argument.
 */
AST_T *parser_parse_function_argument(parser_T *parser);

/**
 * Parses a compound ifelse statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the compound statement.
 */
AST_T *parser_parse_ifelse_statement(parser_T *parser);

/**
 * Parses an expressions with precedence statement.
 * @param parser The parser instance.
 * @param precedence The precedence level to use.
 * @return The parsed AST node representing the if statement.
 */
AST_T *parser_parse_expression_with_precedence(parser_T *parser, int precedence);

/**
 * Parses a factor with precedence statement.
 * @param parser The parser instance.
 * @param precedence The precedence level to use.
 * @return The parsed AST node representing the factor.
 */
AST_T *parser_parse_factor_with_precedence(parser_T *parser, int precedence);

/**
 * Parses a nested expression.
 * @param parser The parser instance.
 * @return The parsed AST node representing the nested expression.
 */
AST_T *parser_parse_for_loop(parser_T *parser);

/**
 * Parses a save expression.
 * @param parser The parser instance.
 * @return The parsed AST node representing the nested expression.
 */
AST_T *parser_parse_save_statement(parser_T *parser);

#endif // PARSE_H