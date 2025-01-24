#ifndef SCOPE_H
#define SCOPE_H

#include "../ast/AST.h"

typedef struct SCOPE_STRUCT
{
    AST_FUNCTION_DEFINITION_T **function_definitions;
    size_t function_definitions_size;

    AST_VARIABLE_DEFINITION_T **variable_definitions;
    size_t variable_definitions_size;

    AST_T *result;
} scope_T;

typedef struct SCOPE_STACK_STRUCT
{
    scope_T *scope;
    struct SCOPE_STACK_STRUCT *parent;
} scope_stack_T;

scope_T *init_scope();

scope_stack_T *init_scope_stack();

void free_scope_stack(scope_stack_T *stack);

void print_scope(scope_T *scope);

scope_stack_T *push_scope_to_stack(scope_stack_T *stack, scope_T *scope);

scope_stack_T *pop_scope_from_stack(scope_stack_T *stack);

AST_T *scope_add_function_definition(scope_T *scope, AST_T *fdef);

AST_T *scope_get_function_definition(scope_T *scope, const char *fname);

AST_T *scope_add_variable_definition(scope_T *scope, AST_T *vdef);

AST_T *scope_get_variable_definition(scope_T *scope, const char *name);

#endif // SCOPE_H