#ifndef VISITOR_STATEMENT_H
#define VISITOR_STATEMENT_H

#include "../ast/AST.h"
#include "visitor.h"

/**
 * Visits a compound node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the compound.
 * @return The result of the visit.
 */
AST_T *visitor_visit_compound(visitor_T *visitor, AST_COMPOUND_T *node);

/**
 * Visits a return node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the return.
 * @return The result of the visit.
 */
AST_T *visitor_visit_return(visitor_T *visitor, AST_RETURN_T *node);

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
 * Visits a for loop node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the term.
 * @return The result of the visit.
 */
AST_T *visitor_visit_for_loop(visitor_T *visitor, AST_FOR_LOOP_T *node);

/**
 * Visits a save node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the save.
 * @return The result of the visit.
 */
AST_T *visitor_visit_save(visitor_T *visitor, AST_SAVE_T *node);

/**
 * Visits a term node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the term.
 * @return The result of the visit.
 */
AST_T *visitor_visit_term(visitor_T *visitor, AST_T *node);

/**
 * Visits a factor node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the factor.
 * @return The result of the visit.
 */
AST_T *visitor_visit_factor(visitor_T *visitor, AST_T *node);

#endif // VISITOR_STATEMENT_H
