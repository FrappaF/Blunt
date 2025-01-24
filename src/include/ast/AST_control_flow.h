#ifndef AST_CONTROL_FLOW_H
#define AST_CONTROL_FLOW_H

#include "AST.h"

/**
 * @brief Structure representing an if-branch AST node.
 */
typedef struct AST_IF_ELSE_BRANCH_STRUCT
{
    AST_T base;
    struct AST_STRUCT **if_else_compound_value;
    size_t if_else_compound_size;
} AST_IF_ELSE_BRANCH_T;

/**
 * @brief Structure representing an if AST node.
 */
typedef struct AST_IF_STRUCT
{
    AST_T base;
    struct AST_STRUCT *if_condition;
    struct AST_STRUCT *if_body;
} AST_IF_T;

/**
 * @brief Structure representing an else AST node.
 */
typedef struct AST_ELSE_STRUCT
{
    AST_T base;
    struct AST_STRUCT *else_body;
} AST_ELSE_T;

/**
 * @brief Structure representing an elseif AST node.
 */
typedef struct AST_ELSEIF_STRUCT
{
    AST_T base;
    struct AST_STRUCT *elseif_condition;
    struct AST_STRUCT *elseif_body;
} AST_ELSEIF_T;

/**
 * @brief Structure representing a for loop AST node.
 */
typedef struct AST_FOR_LOOP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *for_loop_variable;
    struct AST_STRUCT *for_loop_condition;
    struct AST_STRUCT *for_loop_increment;
    struct AST_STRUCT *for_loop_body;
} AST_FOR_LOOP_T;

#endif // AST_CONTROL_FLOW_H
