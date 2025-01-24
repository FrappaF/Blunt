#include "include/visitor_function.h"
#include "include/logger.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>
#include "include/AST.h"

AST_T *visitor_visit_function_call(visitor_T *visitor, AST_FUNCTION_CALL_T *node)
{
    if (!node->function_call_name)
    {
        log_error("Function call name is NULL\n");
        exit(1);
    }

    LOG_PRINT("Visiting function call\n");
    LOG_PRINT("Function name: %s\n", node->function_call_name);

    if (strcmp(node->function_call_name, "print") == 0)
    {
        builtin_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
        return init_ast(AST_NOOP);
    }
    else if (strcmp(node->function_call_name, "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(node->function_call_name, "println") == 0)
    {
        builtin_println(visitor, node->function_call_arguments, node->function_call_arguments_size);
        return init_ast(AST_NOOP);
    }
    else if (strcmp(node->function_call_name, "len") == 0)
    {
        int length = builtin_len(visitor, node->function_call_arguments[0]);
        AST_INT_T *result = (AST_INT_T *)init_ast(AST_INT);
        result->int_value = length;
        return (AST_T *)result;
    }
    else
    {
        AST_FUNCTION_DEFINITION_T *function_definition = NULL;
        scope_stack_T *current_scope_stack = visitor->scope_stack;

        while (current_scope_stack)
        {
            function_definition = scope_get_function_definition(current_scope_stack->scope, node->function_call_name);
            if (function_definition)
            {
                LOG_PRINT("Function definition found in scope %p\n", current_scope_stack->scope);
                break;
            }
            current_scope_stack = current_scope_stack->parent;
        }

        if (!function_definition)
        {
            function_definition = scope_get_function_definition(visitor->global_scope, node->function_call_name);
            if (function_definition)
            {
                LOG_PRINT("Function definition found in global scope %p\n", visitor->global_scope);
            }
        }

        if (function_definition)
        {
            if (!function_definition->function_definition_body)
            {
                log_error("Function definition body is NULL\n");
                exit(1);
            }

            // Make a copy for runtime function definition
            AST_RUNTIME_FUNCTION_DEFINITION_T *runtime_function_definition = (AST_RUNTIME_FUNCTION_DEFINITION_T *)init_ast(AST_RUNTIME_FUNCTION_DEFINITION);

            runtime_function_definition->runtime_function_definition_name = function_definition->function_definition_name;
            runtime_function_definition->runtime_function_definition_body = function_definition->function_definition_body;

            visitor->current_function = runtime_function_definition;

            size_t arguments_size = function_definition->function_definition_arguments_size;
            AST_VARIABLE_DEFINITION_T **arguments = calloc(arguments_size, sizeof(struct AST_VARIABLE_DEFINITION_T *));

            for (size_t i = 0; i < arguments_size; i++)
            {
                AST_VARIABLE_DEFINITION_T *argument_copy = calloc(1, sizeof(struct AST_VARIABLE_DEFINITION_STRUCT));
                if (!argument_copy)
                {
                    log_error("Failed to allocate memory for argument_copy\n");
                    exit(1);
                }

                AST_VARIABLE_T *original_argument = function_definition->function_definition_arguments[i];

                if (i >= node->function_call_arguments_size)
                {
                    log_error("Not enough arguments provided for function '%s' expected argument '%s'\n",
                              node->function_call_name,
                              original_argument->variable_name);
                    exit(1);
                }

                LOG_PRINT("Passed argument type: %s\n", ast_type_to_string(node->function_call_arguments[i]->type));

                argument_copy->variable_definition_variable_name = strdup(original_argument->variable_name);
                argument_copy->variable_definition_value = visitor_visit(visitor, node->function_call_arguments[i]);
                argument_copy->variable_definition_variable_count = (AST_VARIABLE_COUNT_T *)init_ast(AST_VARIABLE_COUNT);
                ((AST_VARIABLE_COUNT_T *)(argument_copy->variable_definition_variable_count))->variable_count_value = 1;

                // Check if function call argument is a variable
                if (node->function_call_arguments[i]->type == AST_VARIABLE)
                {
                    int count = visitor_get_variable_count(visitor, (AST_VARIABLE_T *)node->function_call_arguments[i]);
                    ((AST_VARIABLE_COUNT_T *)(argument_copy->variable_definition_variable_count))->variable_count_value = count;
                }

                arguments[i] = argument_copy;

                LOG_PRINT("Adding argument %s [%s] (count: %lu)\n",
                          argument_copy->variable_definition_variable_name,
                          ast_type_to_string(argument_copy->variable_definition_value->type),
                          ((AST_VARIABLE_COUNT_T *)(argument_copy->variable_definition_variable_count))->variable_count_value);
            }

            visitor->scope_stack = push_scope_to_stack(visitor->scope_stack, init_scope());

            for (size_t i = 0; i < arguments_size; i++)
            {
                LOG_PRINT("Adding argument to scope: %s\n",
                          (arguments[i])->variable_definition_variable_name);

                LOG_PRINT("Argument count: %lu\n",
                          ((AST_VARIABLE_COUNT_T *)((arguments[i])->variable_definition_variable_count))->variable_count_value);

                // Add the argument to runtime function definition variables
                runtime_function_definition->function_definition_variables_size++;
                runtime_function_definition->function_definition_variables = realloc(
                    runtime_function_definition->function_definition_variables,
                    sizeof(struct AST_VARIABLE_DEFINITION_STRUCT *) * runtime_function_definition->function_definition_variables_size);
                runtime_function_definition->function_definition_variables[runtime_function_definition->function_definition_variables_size - 1] = arguments[i];

                visitor_add_variable_definition(visitor, (arguments[i]));
            }

            LOG_PRINT("New scope after adding arguments\n");
            print_scope(visitor->scope_stack->scope);

            AST_T *function_body = visitor_visit(visitor, function_definition->function_definition_body);

            if (function_body->type == AST_RETURN)
            {
                LOG_PRINT("Visiting return value from function call\n");
                AST_T *result = visitor_visit(visitor, ((AST_RETURN_T *)function_body)->return_value);

                visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);
                return result;
            }

            visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);
            visitor->current_function = NULL;

            LOG_PRINT("Returning runtime function definition\n");
            ast_print(runtime_function_definition, 0);

            return runtime_function_definition;
        }
    }

    log_error("Function '%s' not defined\n", node->function_call_name);
    exit(1);
}

AST_T *visitor_visit_runtime_function_call(visitor_T *visitor, AST_RUNTIME_FUNCTION_DEFINITION_T *node, AST_FUNCTION_CALL_T *function_call)
{
    LOG_PRINT("Visiting runtime function call\n");

    char *function_to_call_name = function_call->function_call_name;

    AST_FUNCTION_DEFINITION_T *call_definition = NULL;

    visitor->current_function = node;

    if (node->runtime_function_definition_body->type == AST_FUNCTION_DEFINITION)
    {
        AST_FUNCTION_DEFINITION_T *nested_function_definition = (AST_FUNCTION_DEFINITION_T *)node->runtime_function_definition_body;
        if (strcmp(nested_function_definition->function_definition_name, function_to_call_name) == 0)
        {
            call_definition = nested_function_definition;
        }
    }

    else if (node->runtime_function_definition_body->type == AST_COMPOUND)
    {
        AST_COMPOUND_T *compound = (AST_COMPOUND_T *)node->runtime_function_definition_body;
        for (size_t i = 0; i < compound->compound_size; i++)
        {
            AST_T *statement = compound->compound_value[i];
            if (statement->type == AST_FUNCTION_DEFINITION)
            {
                AST_FUNCTION_DEFINITION_T *nested_function_definition = (AST_FUNCTION_DEFINITION_T *)statement;
                if (strcmp(nested_function_definition->function_definition_name, function_to_call_name) == 0)
                {
                    LOG_PRINT("Function definition found:\n");
                    ast_print(nested_function_definition, 0);
                    call_definition = nested_function_definition;
                }
            }
        }
    }

    if (call_definition == NULL)
    {
        log_error("Function definition for %s not found\n", function_to_call_name);
        exit(1);
    }

    LOG_PRINT("Function definition found\n");

    // Add function definition to new scope stack
    visitor->scope_stack = push_scope_to_stack(visitor->scope_stack, init_scope());
    visitor_add_function_definition(visitor, (AST_T *)call_definition);

    // Add function variables to new scope stack
    LOG_PRINT("Adding function variables to scope (length: %lu)\n", node->function_definition_variables_size);
    for (size_t i = 0; i < node->function_definition_variables_size; i++)
    {
        AST_VARIABLE_DEFINITION_T *variable_definition = node->function_definition_variables[i];
        LOG_PRINT("Adding function variable %s (count %d) to scope\n", variable_definition->variable_definition_variable_name, ((AST_VARIABLE_COUNT_T *)(variable_definition->variable_definition_variable_count))->variable_count_value);
        visitor_add_variable_definition(visitor, (AST_T *)variable_definition);
    }

    AST_T *result = visitor_visit_function_call(visitor, function_call);

    // Pop scope stack
    visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);
    return result;
}

AST_T *visitor_visit_function_call_from_definition(visitor_T *visitor, AST_RUNTIME_FUNCTION_DEFINITION_T *function_definition, AST_FUNCTION_CALL_T *function_call)
{
    LOG_PRINT("Visiting function call from definition\n");

    char *function_to_call_name = function_call->function_call_name;

    AST_FUNCTION_DEFINITION_T *call_definition = NULL;

    if (function_definition->runtime_function_definition_body->type == AST_FUNCTION_DEFINITION)
    {
        AST_FUNCTION_DEFINITION_T *nested_function_definition = (AST_FUNCTION_DEFINITION_T *)function_definition->runtime_function_definition_body;
        if (strcmp(nested_function_definition->function_definition_name, function_to_call_name) == 0)
        {
            call_definition = nested_function_definition;
        }
    }

    else if (function_definition->runtime_function_definition_body->type == AST_COMPOUND)
    {
        AST_COMPOUND_T *compound = (AST_COMPOUND_T *)function_definition->runtime_function_definition_body;
        for (size_t i = 0; i < compound->compound_size; i++)
        {
            AST_T *statement = compound->compound_value[i];
            if (statement->type == AST_FUNCTION_DEFINITION)
            {
                AST_FUNCTION_DEFINITION_T *nested_function_definition = (AST_FUNCTION_DEFINITION_T *)statement;
                if (strcmp(nested_function_definition->function_definition_name, function_to_call_name) == 0)
                {
                    call_definition = nested_function_definition;
                }
            }
        }
    }

    if (call_definition == NULL)
    {
        log_error("Function definition for %s not found\n", function_to_call_name);
        exit(1);
    }

    LOG_PRINT("Function definition found\n");

    // Add function definition to new scope stack
    visitor->scope_stack = push_scope_to_stack(visitor->scope_stack, init_scope());
    visitor_add_function_definition(visitor, (AST_T *)call_definition);

    // Add function variables to new scope stack
    LOG_PRINT("Adding function variables to scope (length: %lu)\n", function_definition->function_definition_variables_size);
    for (size_t i = 0; i < function_definition->function_definition_variables_size; i++)
    {
        AST_VARIABLE_DEFINITION_T *variable_definition = function_definition->function_definition_variables[i];
        LOG_PRINT("Adding function variable %s (count %d) to scope\n", variable_definition->variable_definition_variable_name, ((AST_VARIABLE_COUNT_T *)(variable_definition->variable_definition_variable_count))->variable_count_value);
        visitor_add_variable_definition(visitor, (AST_T *)variable_definition);
    }

    AST_T *result = visitor_visit_function_call(visitor, function_call);

    // Pop scope stack
    visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);
    return result;
}

AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_FUNCTION_DEFINITION_T *node)
{
    if (!node->function_definition_name)
    {
        log_error("Function definition name is NULL\n");
        exit(1);
    }

    if (!node->function_definition_body)
    {
        log_error("Function definition body is NULL\n");
        exit(1);
    }

    LOG_PRINT("Visiting function definition\n");
    LOG_PRINT("Function name: %s\n", node->function_definition_name);
    LOG_PRINT("Function arguments size: %lu\n", node->function_definition_arguments_size);

    for (size_t i = 0; i < node->function_definition_arguments_size; i++)
    {

        AST_VARIABLE_T *argument = node->function_definition_arguments[i];
        if (!argument)
        {
            log_error("Function definition argument %lu is NULL\n", i);
            exit(1);
        }

        LOG_PRINT("Function argument %lu [%s]\n", i, ast_type_to_string(argument->base.type));

        if (!argument->variable_name)
        {
            log_error("Function definition argument name is NULL\n");
            exit(1);
        }

        LOG_PRINT("Function argument %lu: %s\n", i, argument->variable_name);
    }

    visitor_add_function_definition(visitor, (AST_T *)node);

    return (AST_T *)node;
}
