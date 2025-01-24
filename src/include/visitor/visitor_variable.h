#ifndef VISITOR_VARIABLE_H
#define VISITOR_VARIABLE_H

#include "../ast/AST.h"
#include "visitor.h"

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
 * Visits a variable with max index count node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable count.
 * @return The result of the visit.
 */
AST_T *visitor_visit_last_variable(visitor_T *visitor, AST_VARIABLE_T *node);

/**
 * Visits a variable with dot dot node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the variable count.
 * @return The result of the visit.
 */
AST_T *visitor_visit_variable_with_dot_dot(visitor_T *visitor, AST_VARIABLE_T *node, int first_index, int last_index);

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

#endif // VISITOR_VARIABLE_H
