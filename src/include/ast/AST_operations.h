#ifndef AST_OPERATIONS_H
#define AST_OPERATIONS_H

#include "AST.h"

/**
 * @brief Structure representing an addition operation AST node.
 */
typedef struct AST_ADD_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_ADD_OP_T;

/**
 * @brief Structure representing a subtraction operation AST node.
 */
typedef struct AST_SUB_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_SUB_OP_T;

/**
 * @brief Structure representing a multiplication operation AST node.
 */
typedef struct AST_MUL_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_MUL_OP_T;

/**
 * @brief Structure representing a division operation AST node.
 */
typedef struct AST_DIV_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_DIV_OP_T;

/**
 * @brief Structure representing an AND operation AST node.
 */
typedef struct AST_AND_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_AND_OP_T;

/**
 * @brief Structure representing an OR operation AST node.
 */
typedef struct AST_OR_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_OR_OP_T;

/**
 * @brief Structure representing a greater than operation AST node.
 */
typedef struct AST_GT_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_GT_OP_T;

/**
 * @brief Structure representing a less than operation AST node.
 */
typedef struct AST_LT_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_LT_OP_T;

/**
 * @brief Structure representing a greater than or equal to operation AST node.
 */
typedef struct AST_GTE_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_GTE_OP_T;

/**
 * @brief Structure representing a less than or equal to operation AST node.
 */
typedef struct AST_LTE_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_LTE_OP_T;

/**
 * @brief Structure representing an equal operation AST node.
 */
typedef struct AST_EQUAL_OP_STRUCT
{
    AST_T base;
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
} AST_EQUAL_OP_T;

/**
 * @brief Structure representing a not operation AST node.
 */
typedef struct AST_NOT_STRUCT
{
    AST_T base;
    struct AST_STRUCT *not_expression;
} AST_NOT_T;

#endif // AST_OPERATIONS_H
