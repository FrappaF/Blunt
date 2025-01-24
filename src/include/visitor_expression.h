#ifndef VISITOR_EXPRESSION_H
#define VISITOR_EXPRESSION_H

#include "AST.h"
#include "visitor.h"

/**
 * Visits a string node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the string.
 * @return The result of the visit.
 */
AST_T *visitor_visit_string(visitor_T *visitor, AST_STRING_T *node);

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
 * Visits a dot expression node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the dot.
 * @return The result of the visit.
 */
AST_T *visitor_visit_dot_expression(visitor_T *visitor, AST_DOT_EXPRESSION_T *node);

/**
 * Visits a dot dot expresion node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the nested expression.
 * @return The result of the visit.
 */
AST_T *visitor_visit_dot_dot_expression(visitor_T *visitor, AST_DOT_DOT_EXPRESSION_T *node);

/**
 * Visits a dot dot annotation node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the dot dot.
 * @return The result of the visit.
 */
AST_T *visitor_visit_dot_dot(visitor_T *visitor, AST_DOT_DOT_T *node);

#endif // VISITOR_EXPRESSION_H
