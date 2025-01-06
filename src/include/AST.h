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
 * @brief Structure representing a dot expression AST node.
 */
typedef struct AST_DOT_EXPRESSION_STRUCT
{
    AST_T base;
    char *dot_expression_variable_name;
    struct AST_STRUCT *dot_index;
} AST_DOT_EXPRESSION_T;

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
    struct AST_VARIABLE_DEFINITION_T **function_definition_arguments;
    size_t function_definition_arguments_size;
} AST_FUNCTION_DEFINITION_T;

/**
 * @brief Structure representing a return AST node.
 */
typedef struct AST_RETURN_STRUCT
{
    AST_T base;
    struct AST_STRUCT *return_value;
} AST_RETURN_T;

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
 * @brief Structure representing an if-branch AST node.
 */
typedef struct AST_IF_ELSE_BRANCH_STRUCT
{
    AST_T base;
    struct AST_STRUCT **if_else_compound_value;
    size_t if_else_compound_size;
} AST_IF_ELSE_BRANCH_T;

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

/**
 * @brief Structure representing a nested expression AST node.
 */
typedef struct AST_NESTED_EXPRESSION_STRUCT
{
    AST_T base;
    struct AST_STRUCT *nested_expression;
} AST_NESTED_EXPRESSION_T;

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

#endif // AST_H