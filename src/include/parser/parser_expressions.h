#ifndef PARSER_EXPRESSIONS_H
#define PARSER_EXPRESSIONS_H

#include "parser.h"

/**
 * Parses an expression based on the current token type.
 * @param parser The parser instance.
 * @return The parsed AST node representing the expression.
 */
AST_T *parser_parse_expression(parser_T *parser);

/**
 * Parses a factor.
 * @param parser The parser instance.
 * @return The parsed AST node representing the factor.
 */
AST_T *parser_parse_factor(parser_T *parser);

/**
 * Parses a function call.
 * @param parser The parser instance.
 * @return The parsed AST node representing the function call.
 */
AST_T *parser_parse_function_call(parser_T *parser);

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
 * Parses an expression with precedence.
 * @param parser The parser instance.
 * @param precedence The precedence level to use.
 * @return The parsed AST node representing the expression.
 */
AST_T *parser_parse_expression_with_precedence(parser_T *parser, int precedence);

/**
 * Parses a factor with precedence.
 * @param parser The parser instance.
 * @param precedence The precedence level to use.
 * @return The parsed AST node representing the factor.
 */
AST_T *parser_parse_factor_with_precedence(parser_T *parser, int precedence);

#endif // PARSER_EXPRESSIONS_H
