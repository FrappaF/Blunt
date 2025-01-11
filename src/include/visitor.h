#ifndef VISITOR_H
#define VISITOR_H

#include "AST.h"
#include "scope.h"
#include <stdlib.h>

typedef struct VISITOR_STRUCT
{
    scope_T *global_scope;
    scope_stack_T *scope_stack;

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
 * Visits a variable definition node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable definition.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_VARIABLE_DEFINITION_T *node);

/**
 * Visits a variable node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable(visitor_T *visitor, AST_VARIABLE_T *node);

/**
 * Visits a variable node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable.
 * @param index The index of the variable.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable_with_index(visitor_T *visitor, AST_VARIABLE_T *node, int index);

/**
 * Visits a variable assignment node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable assignment.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable_assignment(visitor_T *visitor, AST_VARIABLE_ASSIGNMENT_T *node);

/**
 * Visits a variable assignment node with specific index in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable assignment.
 * @param index The index of the variable.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable_assignment_with_index(visitor_T *visitor, AST_VARIABLE_ASSIGNMENT_T *node, int index);

/**
 * Visits a string node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the string.
 * @return The result of the visit.
 */
AST_T *visitor_visit_string(visitor_T *visitor, AST_STRING_T *node);

/**
 * Visits a compound node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the compound.
 * @return The result of the visit.
 */
AST_T *visitor_visit_compound(visitor_T *visitor, AST_COMPOUND_T *node);

/**
 * Visits a function call node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the function call.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_call(visitor_T *visitor, AST_FUNCTION_CALL_T *node);

/**
 * Visits a function definition node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the function definition.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_FUNCTION_DEFINITION_T *node);

/**
 * Visits a return node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the return.
 * @return The result of the visit.
 */
AST_T *visitor_visit_return(visitor_T *visitor, AST_RETURN_T *node);

/**
 * Visits an integer node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the integer.
 * @return The result of the visit.
 */
AST_T *visitor_visit_int(visitor_T *visitor, AST_INT_T *node);

/**
 * Visits an array node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the array.
 * @return The result of the visit.
 */
AST_T *visitor_visit_array(visitor_T *visitor, AST_ARRAY_T *node);

/**
 * Visits a if branch in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the if.
 * @return The result of the visit.
 */
AST_T *visitor_visit_if_branch(visitor_T *visitor, AST_IF_ELSE_BRANCH_T *node);

/**
 * Visits a not node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the not.
 * @return The result of the visit.
 */
AST_T *visitor_visit_not(visitor_T *visitor, AST_NOT_T *node);

/**
 * Visits a dot expression node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the dot.
 * @return The result of the visit.
 */
AST_T *visitor_visit_dot_expression(visitor_T *visitor, AST_DOT_EXPRESSION_T *node);

/**
 * Visits a for loop node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the term.
 * @return The result of the visit.
 */
AST_T *visitor_visit_for_loop(visitor_T *visitor, AST_FOR_LOOP_T *node);

/**
 * Get a variable definition from the visitor.
 * @param visitor The visitor.
 * @param variable_definition The variable definition to add.
 */
AST_VARIABLE_DEFINITION_T *visitor_get_variable_definition(visitor_T *visitor, char *variable_name);

/**
 * Gets the value of a node in the AST.
 * @param visitor The visitor.
 * @param node The AST node to get the value from.
 * @return The value of the node.
 */
int visitor_get_node_value(visitor_T *visitor, AST_T *node);

#endif // VISITOR_H