#ifndef PARSER_STATEMENTS_H
#define PARSER_STATEMENTS_H

#include "parser.h"

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
 * Parses a variable definition statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the variable definition.
 */
AST_T *parser_parse_variable_definition(parser_T *parser);

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
 * Parses a compound ifelse statement.
 * @param parser The parser instance.
 * @return The parsed AST node representing the compound statement.
 */
AST_T *parser_parse_ifelse_statement(parser_T *parser);

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

#endif // PARSER_STATEMENTS_H
