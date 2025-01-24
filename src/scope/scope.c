#include "../include/io/logger.h"
#include "../include/scope/scope.h"
#include <string.h>

scope_T *init_scope()
{
    scope_T *scope = calloc(1, sizeof(struct SCOPE_STRUCT));

    scope->function_definitions = (void *)0;
    scope->function_definitions_size = 0;

    scope->variable_definitions = (void *)0;
    scope->variable_definitions_size = 0;

    scope->result = (void *)0;

    return scope;
}

scope_stack_T *init_scope_stack()
{
    scope_stack_T *stack = calloc(1, sizeof(struct SCOPE_STACK_STRUCT));
    stack->scope = (void *)0;
    stack->parent = (void *)0;

    return stack;
}

void free_scope_stack(scope_stack_T *stack)
{
    if (stack->parent != (void *)0)
    {
        free_scope_stack(stack->parent);
    }

    free(stack);
}

scope_stack_T *push_scope_to_stack(scope_stack_T *stack, scope_T *scope)
{
    scope_stack_T *new_stack = init_scope_stack();
    new_stack->scope = scope;
    new_stack->parent = stack;

    LOG_PRINT("Pushing scope to stack\n");
    LOG_PRINT("Parent scope: %p\n", stack->scope);
    LOG_PRINT("New scope: %p\n", new_stack->scope);

    return new_stack;
}

scope_stack_T *pop_scope_from_stack(scope_stack_T *stack)
{
    scope_stack_T *parent_stack = stack->parent;

    LOG_PRINT("Popping scope from stack\n");
    LOG_PRINT("Parent scope: %p\n", parent_stack ? parent_stack->scope : NULL);
    LOG_PRINT("Popped scope: %p\n", stack->scope);

    free(stack);
    return parent_stack;
}

void print_scope(scope_T *scope)
{
    if (scope == (void *)0)
    {
        return;
    }
    LOG_PRINT("------ Scope ------\n");
    LOG_PRINT("Scope %p\n", scope);
    LOG_PRINT("Function definitions size: %lu\n", scope->function_definitions_size);
    for (size_t i = 0; i < scope->function_definitions_size; i++)
    {
        AST_FUNCTION_DEFINITION_T *fdef = scope->function_definitions[i];
        LOG_PRINT("Function definition %lu: %s\n", i, fdef->function_definition_name);
    }

    LOG_PRINT("Variable definitions size: %lu\n", scope->variable_definitions_size);
    for (size_t i = 0; i < scope->variable_definitions_size; i++)
    {
        AST_VARIABLE_DEFINITION_T *vdef = scope->variable_definitions[i];
        LOG_PRINT("Variable definition %lu: %s \n", i, vdef->variable_definition_variable_name);
    }
    LOG_PRINT("-------------------\n");
}

AST_T *scope_add_function_definition(scope_T *scope, AST_T *fdef)
{
    scope->function_definitions_size += 1;

    if (scope->function_definitions == (void *)0)
    {
        scope->function_definitions = calloc(1, sizeof(struct AST_STRUCT *));
    }
    else
    {
        scope->function_definitions =
            realloc(
                scope->function_definitions,
                scope->function_definitions_size * sizeof(struct AST_STRUCT **));
    }

    scope->function_definitions[scope->function_definitions_size - 1] =
        fdef;

    return fdef;
}

AST_T *scope_get_function_definition(scope_T *scope, const char *fname)
{
    if (scope == (void *)0)
    {
        return (void *)0;
    }
    for (int i = 0; i < scope->function_definitions_size; i++)
    {
        AST_FUNCTION_DEFINITION_T *fdef = scope->function_definitions[i];

        if (strcmp(fdef->function_definition_name, fname) == 0)
        {
            return fdef;
        }
    }

    return (void *)0;
}

AST_T *scope_add_variable_definition(scope_T *scope, AST_T *vdef)
{
    if (scope->variable_definitions == (void *)0)
    {
        scope->variable_definitions = calloc(1, sizeof(struct AST_STRUCT *));
        scope->variable_definitions[0] = vdef;
        scope->variable_definitions_size += 1;
    }
    else
    {
        scope->variable_definitions_size += 1;
        scope->variable_definitions = realloc(
            scope->variable_definitions,
            scope->variable_definitions_size * sizeof(struct AST_STRUCT *));
        scope->variable_definitions[scope->variable_definitions_size - 1] = vdef;
    }

    return vdef;
}

AST_T *scope_get_variable_definition(scope_T *scope, const char *name)
{
    if (scope == (void *)0)
    {
        return (void *)0;
    }
    for (int i = 0; i < scope->variable_definitions_size; i++)
    {
        AST_VARIABLE_DEFINITION_T *vdef = scope->variable_definitions[i];
        LOG_PRINT("Variable definition in scope %p: %s\n", scope, vdef->variable_definition_variable_name);
        if (strcmp(vdef->variable_definition_variable_name, name) == 0)
        {
            return vdef;
        }
    }

    return (void *)0;
}