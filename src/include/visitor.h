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
AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_T *node);

/**
 * Visits a variable node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable(visitor_T *visitor, AST_T *node);

/**
 * Visits a string node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the string.
 * @return The result of the visit.
 */
AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node);

/**
 * Visits a compound node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the compound.
 * @return The result of the visit.
 */
AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node);

/**
 * Visits a function call node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the function call.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_call(visitor_T *visitor, AST_T *node);

/**
 * Visits a function definition node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the function definition.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_T *node);

/**
 * Visits a return node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the return.
 * @return The result of the visit.
 */
AST_T *visitor_visit_return(visitor_T *visitor, AST_T *node);

/**
 * Visits an integer node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the integer.
 * @return The result of the visit.
 */
AST_T *visitor_visit_int(visitor_T *visitor, AST_T *node);

/**
 * Visits a if branch in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the term.
 * @return The result of the visit.
 */
AST_T *visitor_visit_if_branch(visitor_T *visitor, AST_T *node);

/**
 * Visits a not node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the term.
 * @return The result of the visit.
 */
AST_T *visitor_visit_not(visitor_T *visitor, AST_T *node);

/**
 * Copy an AST node.
 * @param node The AST node representing the term.
 * @return The result of the visit.
 */
AST_T *copy_ast(AST_T *node);

/**
 * Gets the value of a node in the AST.
 * @param visitor The visitor.
 * @param node The AST node to get the value from.
 * @return The value of the node.
 */
int visitor_get_node_value(visitor_T *visitor, AST_T *node);

#endif // VISITOR_H