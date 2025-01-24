#ifndef AST_VARIABLE_H
#define AST_VARIABLE_H

#include "AST.h"

/**
 * @brief Structure representing a variable count AST node.
 */
typedef struct AST_VARIABLE_COUNT_STRUCT
{
    AST_T base;
    int variable_count_value;
} AST_VARIABLE_COUNT_T;

/**
 * @brief Structure representing a variable definition AST node.
 */
typedef struct AST_VARIABLE_DEFINITION_STRUCT
{
    AST_T base;
    char *variable_definition_variable_name;
    struct AST_STRUCT *variable_definition_value;
    struct AST_VARIABLE_COUNT_T *variable_definition_variable_count;
} AST_VARIABLE_DEFINITION_T;

/**
 * @brief Structure representing a variable AST node.
 */
typedef struct AST_VARIABLE_STRUCT
{
    AST_T base;
    char *variable_name;
} AST_VARIABLE_T;

/**
 * @brief Structure representing a variable assignment AST node.
 */
typedef struct AST_VARIABLE_ASSIGNMENT_STRUCT
{
    AST_T base;
    char *variable_assignment_name;
    struct AST_STRUCT *variable_assignment_value;
} AST_VARIABLE_ASSIGNMENT_T;

#endif // AST_VARIABLE_H
