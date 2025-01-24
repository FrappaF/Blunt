#ifndef AST_MISC_H
#define AST_MISC_H

#include "AST.h"

/**
 * @brief Structure representing a string AST node.
 */
typedef struct AST_STRING_STRUCT
{
    AST_T base;
    char *string_value;
} AST_STRING_T;

/**
 * @brief Structure representing an integer AST node.
 */
typedef struct AST_INT_STRUCT
{
    AST_T base;
    int int_value;
} AST_INT_T;

/**
 * @brief Structure representing an array AST node.
 */
typedef struct AST_ARRAY_STRUCT
{
    AST_T base;
    struct AST_STRUCT **array_value;
    size_t array_size;
} AST_ARRAY_T;

/**
 * @brief Structure representing a compound AST node.
 */
typedef struct AST_COMPOUND_STRUCT
{
    AST_T base;
    struct AST_STRUCT **compound_value;
    size_t compound_size;
} AST_COMPOUND_T;

/**
 * @brief Structure representing a nested expression AST node.
 */
typedef struct AST_NESTED_EXPRESSION_STRUCT
{
    AST_T base;
    struct AST_STRUCT *nested_expression;
} AST_NESTED_EXPRESSION_T;

/**
 * @brief Structure representing a return AST node.
 */
typedef struct AST_RETURN_STRUCT
{
    AST_T base;
    struct AST_STRUCT *return_value;
} AST_RETURN_T;

/**
 * @brief Structure representing a dot expression AST node.
 */
typedef struct AST_DOT_EXPRESSION_STRUCT
{
    AST_T base;
    char *dot_expression_variable_name;
    struct AST_STRUCT *dot_index;
} AST_DOT_EXPRESSION_T;

/**
 * @brief Structure representing a dot dot annotation AST node.
 */
typedef struct AST_DOT_DOT_STRUCT
{
    AST_T base;
} AST_DOT_DOT_T;

/**
 * @brief Structure representing a dot dot expression AST node.
 * I.e. "string".3.6 -> "ing"
 */
typedef struct AST_DOT_DOT_EXPRESSION_STRUCT
{
    AST_T base;
    char *dot_dot_expression_variable_name;
    struct AST_STRUCT *dot_dot_first_index;
    struct AST_STRUCT *dot_dot_last_index;
} AST_DOT_DOT_EXPRESSION_T;

/**
 * @brief Structure representing a save AST node.
 */
typedef struct AST_SAVE_STRUCT
{
    AST_T base;
    struct AST_VARIABLE_T *save_value;
} AST_SAVE_T;

#endif // AST_MISC_H
