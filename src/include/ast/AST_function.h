#ifndef AST_FUNCTION_H
#define AST_FUNCTION_H

#include "AST.h"

/**
 * @brief Structure representing a function call AST node.
 */
typedef struct AST_FUNCTION_CALL_STRUCT
{
    AST_T base;
    char *function_call_name;
    struct AST_STRUCT **function_call_arguments;
    size_t function_call_arguments_size;
} AST_FUNCTION_CALL_T;

/**
 * @brief Structure representing a function definition AST node.
 */
typedef struct AST_FUNCTION_DEFINITION_STRUCT
{
    AST_T base;
    char *function_definition_name;
    struct AST_STRUCT *function_definition_body;
    struct AST_VARIABLE_T **function_definition_arguments;
    size_t function_definition_arguments_size;
} AST_FUNCTION_DEFINITION_T;

/**
 * @brief Structure representing a runtime function definition AST node.
 */
typedef struct AST_RUNTIME_FUNCTION_DEFINITION_STRUCT
{
    AST_T base;
    char *runtime_function_definition_name;
    struct AST_STRUCT *runtime_function_definition_body;
    struct AST_VARIABLE_DEFINITION_T **function_definition_variables;
    size_t function_definition_variables_size;
} AST_RUNTIME_FUNCTION_DEFINITION_T;

#endif // AST_FUNCTION_H
