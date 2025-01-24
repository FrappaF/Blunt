#ifndef VISITOR_H
#define VISITOR_H

#include "../ast/AST.h"
#include "../scope/scope.h"
#include <stdlib.h>

/**
 * @brief Structure representing a visitor.
 */
typedef struct VISITOR_STRUCT
{
    scope_T *global_scope;
    scope_stack_T *scope_stack;
    AST_RUNTIME_FUNCTION_DEFINITION_T *current_function;
} visitor_T;

/**
 * Initializes the visitor.
 * @return A pointer to the initialized visitor.
 */
visitor_T *init_visitor();

/**
 * Visits a node in the AST.
 * @param visitor The visitor.
 * @param node The AST node to visit.
 * @return The result of the visit.
 */
AST_T *visitor_visit(visitor_T *visitor, AST_T *node);

/**
 * Gets a variable definition from the visitor.
 * @param visitor The visitor.
 * @param variable_name The name of the variable.
 * @return The variable definition.
 */
AST_VARIABLE_DEFINITION_T *visitor_get_variable_definition(visitor_T *visitor, char *variable_name);

// Function declarations

/**
 * Gets the length of a node.
 * @param visitor The visitor.
 * @param node The AST node.
 * @return The length of the node.
 */
int builtin_len(visitor_T *visitor, AST_T *node);

/**
 * Prints the arguments.
 * @param visitor The visitor.
 * @param arguments The arguments.
 * @param arguments_size The size of the arguments.
 */
void builtin_print(visitor_T *visitor, AST_T **arguments, size_t arguments_size);

/**
 * Prints the arguments with "\n" at the end.
 * @param visitor The visitor.
 * @param arguments The arguments.
 * @param arguments_size The size of the arguments.
 */
void builtin_println(visitor_T *visitor, AST_T **arguments, size_t arguments_size);

/**
 * Adds a function definition to the visitor scope.
 * @param visitor The visitor.
 * @param node The AST node representing the function definition.
 */
void visitor_add_function_definition(visitor_T *visitor, AST_T *node);

/**
 * Adds a variable definition to the visitor scope.
 * @param visitor The visitor.
 * @param node The AST node representing the variable definition.
 */
void visitor_add_variable_definition(visitor_T *visitor, AST_T *node);

/**
 * Gets the variable count.
 * @param visitor The visitor.
 * @param node The AST node representing the variable.
 * @return The variable count.
 */
int visitor_get_variable_count(visitor_T *visitor, AST_VARIABLE_T *node);

/**
 * Gets the node value.
 * @param visitor The visitor.
 * @param node The AST node.
 * @return The node value.
 */
int visitor_get_node_value(visitor_T *visitor, AST_T *node);

#include "visitor_variable.h"
#include "visitor_function.h"
#include "visitor_expression.h"
#include "visitor_statement.h"

#endif // VISITOR_H