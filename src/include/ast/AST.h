#ifndef AST_H
#define AST_H

#include <stdlib.h>

enum AST_TYPES
{
    AST_VARIABLE_DEFINITION,
    AST_VARIABLE,
    AST_VARIABLE_COUNT,
    AST_VARIABLE_ASSIGNMENT,
    AST_FUNCTION_DEFINITION,
    AST_RUNTIME_FUNCTION_DEFINITION,
    AST_FUNCTION_CALL,
    AST_RETURN,
    AST_STRING,
    AST_INT,
    AST_ARRAY,
    AST_COMPOUND,
    AST_ADD_OP,
    AST_SUB_OP,
    AST_MUL_OP,
    AST_DIV_OP,
    AST_GT_OP,
    AST_LT_OP,
    AST_GTE_OP,
    AST_LTE_OP,
    AST_AND_OP,
    AST_OR_OP,
    AST_EQUAL_OP,
    AST_NOT,
    AST_NESTED_EXPRESSION,
    AST_IF,
    AST_ELSE,
    AST_ELSEIF,
    AST_IF_ELSE_BRANCH,
    AST_DOT_EXPRESSION,
    AST_DOT_DOT_EXPRESSION,
    AST_FOR_LOOP,
    AST_SAVE,
    AST_DOT_DOT,
    AST_NOOP
};

/**
 * @brief Base structure representing an Abstract Syntax Tree (AST) node.
 */
typedef struct AST_STRUCT
{
    int type;
} AST_T;

/**
 * Initializes an AST node with the given type.
 * @param type The type of the AST node.
 * @return A pointer to the initialized AST node.
 */
AST_T *init_ast(int type);

/**
 * Returns the size of the AST node based on its type.
 * @param ast The AST node to get the size of.
 * @return The size of the AST node and its children.
 */
size_t ast_get_size(AST_T *ast);

/**
 * Returns the string representation of the AST type.
 * @param type The type of the AST node.
 * @return The string representation of the AST type.
 */
const char *ast_type_to_string(int type);

/**
 * Prints the AST node and its children.
 * @param node The AST node to print.
 * @param indent The current indentation level.
 */
void ast_print(AST_T *node, int indent);

#include "AST_variable.h"
#include "AST_function.h"
#include "AST_control_flow.h"
#include "AST_operations.h"
#include "AST_misc.h"

#endif // AST_H