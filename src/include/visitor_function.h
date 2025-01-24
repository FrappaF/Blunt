#ifndef VISITOR_FUNCTION_H
#define VISITOR_FUNCTION_H

#include "AST.h"
#include "visitor.h"

/**
 * Visits a function call node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the function call.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_call(visitor_T *visitor, AST_FUNCTION_CALL_T *node);

/**
 * Visits a runtime function call node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the runtime function call.
 * @return The result of the visit.
 */
AST_T *visitor_visit_runtime_function_call(visitor_T *visitor, AST_RUNTIME_FUNCTION_DEFINITION_T *node, AST_FUNCTION_CALL_T *function_call);

/**
 * Visits a function definition node in the AST.
 * @param visitor The visitor.
 * @param node The AST node representing the function definition.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_FUNCTION_DEFINITION_T *node);

/**
 * Visits a function call from a definition node in the AST.
 * @param visitor The visitor.
 * @param function_definition The AST node representing the function definition.
 * @param function_call The AST node representing the function call.
 * @return The result of the visit.
 */
AST_T *visitor_visit_function_call_from_definition(visitor_T *visitor, AST_RUNTIME_FUNCTION_DEFINITION_T *function_definition, AST_FUNCTION_CALL_T *function_call);

#endif // VISITOR_FUNCTION_H
