#ifndef AST_H
#define AST_H

#include <stdlib.h>

/**
 * @brief Structure representing an Abstract Syntax Tree (AST) node.
 *
 * This structure is used to represent different types of nodes in the AST.
 *
 * @var AST_T::type
 * The type of the AST node, which can be a variable definition, function call, string, variable, or compound.
 *
 * @var AST_T::scope
 * The scope of the AST node.
 *
 * @var AST_T::variable_definition_variable_name
 * The name of the variable being defined (used when type is AST_VARIABLE_DEFINITION).
 *
 * @var AST_T::variable_definition_value
 * The value assigned to the variable being defined (used when type is AST_VARIABLE_DEFINITION).
 *
 * @var AST_T::variable_name
 * The name of the variable (used when type is AST_VARIABLE).
 *
 * @var AST_T::function_call_name
 * The name of the function being called (used when type is AST_FUNCTION_CALL).
 *
 * @var AST_T::function_call_arguments
 * The arguments passed to the function call (used when type is AST_FUNCTION_CALL).
 *
 * @var AST_T::function_call_arguments_size
 * The number of arguments passed to the function call (used when type is AST_FUNCTION_CALL).
 *
 * @var AST_T::string_value
 * The value of the string (used when type is AST_STRING).
 *
 * @var AST_T::compound_value
 * The array of AST nodes that make up the compound statement (used when type is AST_COMPOUND).
 *
 * @var AST_T::compound_size
 * The number of AST nodes in the compound statement (used when type is AST_COMPOUND).
 */
typedef struct AST_STRUCT
{
    enum
    {
        AST_VARIABLE_DEFINITION,
        AST_VARIABLE_COUNT,
        AST_VARIABLE,
        AST_FUNCTION_DEFINITION,
        AST_FUNCTION_CALL,
        AST_RETURN,
        AST_STRING,
        AST_INT,
        AST_COMPOUND,
        AST_MUL,
        AST_DIV,
        AST_PLUS,
        AST_MINUS,
        AST_AND,
        AST_OR,
        AST_NOT,
        AST_EQUAL,
        AST_NESTED_EXPRESSION,
        AST_IF_BRANCH,
        AST_IF,
        AST_ELSE,
        AST_ELSEIF,
        AST_GT,
        AST_LT,
        AST_GTE,
        AST_LTE,
        NOOP
    } type;

    /* AST_VARIABLE_DEFINITION */
    char *variable_definition_variable_name;
    struct AST_STRUCT *variable_definition_value;
    struct AST_STRUCT *variable_definition_variable_count;

    /* AST_VARIABLE */
    char *variable_name;

    /* AST_VARIABLE_COUNT */
    int variable_count;

    /* AST_FUNCTION_CALL */
    char *function_call_name;
    struct AST_STRUCT **function_call_arguments;
    size_t function_call_arguments_size;
    // struct AST_STRUCT *function_call_result;

    /* AST_FUNCTION_DEFINITION */
    char *function_definition_name;
    struct AST_STRUCT *function_definition_body;
    struct AST_STRUCT **function_definition_arguments;
    size_t function_definition_arguments_size;

    /* AST_RETURN */
    struct AST_STRUCT *return_value;

    /* AST_STRING */
    char *string_value;

    /* AST_INT */
    int int_value;

    /* AST_COMPOUND */
    struct AST_STRUCT **compound_value;
    size_t compound_size;

    /* AST_IF_BRANCH */
    struct AST_STRUCT **if_else_compound_value;
    size_t if_else_compound_size;

    /* AST_MUL, AST_DIV, AST_PLUS, AST_MINUS
     * AST_GT, AST_LT, AST_GTE, AST_LTE
     * AST_AND, AST_OR
     */
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
    int result;

    /* AST_NOT */
    struct AST_STRUCT *not_expression;

    /* AST_NESTED_EXPRESSION */
    struct AST_STRUCT *nested_expression;

    /* AST_IF */
    struct AST_STRUCT *if_condition;
    struct AST_STRUCT *if_body;

    /* AST_ELSE */
    struct AST_STRUCT *else_body;

    /* AST_ELSEIF */
    struct AST_STRUCT *elseif_condition;
    struct AST_STRUCT *elseif_body;

} AST_T;

/**
 * Initializes an AST node with the given type.
 * @param type The type of the AST node.
 * @return A pointer to the initialized AST node.
 */
AST_T *init_ast(int type);

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