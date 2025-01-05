#include "include/visitor.h"
#include "include/logger.h"
#include "include/token.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

// Function declarations
static void builtin_print(visitor_T *visitor, AST_T **arguments, size_t arguments_size);
static void builtin_println(visitor_T *visitor, AST_T **arguments, size_t arguments_size);
void visitor_add_function_definition(visitor_T *visitor, AST_T *node);
void visitor_add_variable_definition(visitor_T *visitor, AST_T *node);
AST_T *visitor_visit_factor(visitor_T *visitor, AST_T *node);
AST_T *visitor_visit_term(visitor_T *visitor, AST_T *node);

// Function definitions
visitor_T *init_visitor()
{
    visitor_T *visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
    if (!visitor)
    {
        log_error("Failed to allocate memory for visitor\n");
        exit(1);
    }

    visitor->global_scope = init_scope();
    visitor->scope_stack = init_scope_stack();

    return visitor;
}

AST_T *visitor_visit(visitor_T *visitor, AST_T *node)
{
    if (!node)
    {
        log_error("Node is NULL\n");
        exit(1);
    }

    switch (node->type)
    {
    case AST_VARIABLE_DEFINITION:
        return visitor_visit_variable_definition(visitor, node);
    case AST_VARIABLE:
        return visitor_visit_variable(visitor, node);
    case AST_STRING:
        return visitor_visit_string(visitor, node);
    case AST_INT:
        return visitor_visit_int(visitor, node);
    case AST_COMPOUND:
        return visitor_visit_compound(visitor, node);
    case AST_FUNCTION_CALL:
        return visitor_visit_function_call(visitor, node);
    case AST_FUNCTION_DEFINITION:
        return visitor_visit_function_definition(visitor, node);
    case AST_RETURN:
        return visitor_visit_return(visitor, node);
    case AST_IF_BRANCH:
        return visitor_visit_if_branch(visitor, node);
    case AST_MUL:
    case AST_DIV:
    case AST_PLUS:
    case AST_MINUS:
    case AST_NESTED_EXPRESSION:
        return visitor_visit_term(visitor, node);
    default:
        LOG_PRINT("Node of type: [%s] not supported\n", ast_type_to_string(node->type));
        return init_ast(NOOP);
    }
}

AST_T *visitor_visit_return(visitor_T *visitor, AST_T *node)
{
    if (!node->return_value)
    {
        log_error("Return value is NULL\n");
        exit(1);
    }

    LOG_PRINT("Visiting return [type]: %s\n", ast_type_to_string(node->return_value->type));
    // AST_T *result = visitor_visit(visitor, node->return_value);
    // if (visitor->scope_stack->scope)
    // {
    //     if (visitor->scope_stack->scope->result == (void *)0)
    //     {
    //         LOG_PRINT("Adding result to scope %p\n", visitor->scope_stack->scope);
    //         visitor->scope_stack->scope->result = result;

    //         LOG_PRINT("Adding result to scope %p\n", visitor->scope_stack->scope);
    //         visitor->scope_stack->scope->result = result;
    //     }
    // }
    // else
    // {
    //     LOG_PRINT("Adding result to global scope %p\n", visitor->global_scope);
    //     visitor->global_scope->result = result;
    // }
    return node;
}

AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_T *node)
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
        if (!node->function_definition_arguments[i])
        {
            log_error("Function definition argument %lu is NULL\n", i);
            exit(1);
        }

        LOG_PRINT("Function argument %lu [%s]\n", i, ast_type_to_string(node->function_definition_arguments[i]->type));

        if (!node->function_definition_arguments[i]->variable_name)
        {
            log_error("Function definition argument name is NULL\n");
            exit(1);
        }

        LOG_PRINT("Function argument %lu: %s\n", i, node->function_definition_arguments[i]->variable_name);
    }

    visitor_add_function_definition(visitor, node);

    return node;
}

AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_T *node)
{
    if (!node->variable_definition_variable_name)
    {
        log_error("Variable definition name is NULL\n");
        exit(1);
    }

    LOG_PRINT("Visiting variable definition\n");
    LOG_PRINT("Variable name: %s\n", node->variable_definition_variable_name);

    if (node->variable_definition_variable_count != NULL)
        LOG_PRINT("Variable count: %d\n", node->variable_definition_variable_count->variable_count);

    if (!node->variable_definition_value)
    {
        log_error("Variable definition value is NULL\n");
        exit(1);
    }

    AST_T *variable_value = visitor_visit(visitor, node->variable_definition_value);
    node->variable_definition_value = variable_value;

    visitor_add_variable_definition(visitor, node);

    return node;
}

AST_T *visitor_visit_variable(visitor_T *visitor, AST_T *node)
{
    if (!node->variable_name)
    {
        log_error("Variable name is NULL\n");
        exit(1);
    }

    scope_stack_T *current_scope_stack = visitor->scope_stack;
    while (current_scope_stack)
    {

        AST_T *local_variable_definition = scope_get_variable_definition(current_scope_stack->scope, node->variable_name);
        if (local_variable_definition)
        {
            LOG_PRINT("Local variable found in scope %p: %s [type: %s]\n", current_scope_stack->scope, node->variable_name, ast_type_to_string(local_variable_definition->variable_definition_value->type));
            return local_variable_definition->variable_definition_value;
        }
        current_scope_stack = current_scope_stack->parent;
    }

    AST_T *global_variable_definition = scope_get_variable_definition(visitor->global_scope, node->variable_name);
    if (global_variable_definition)
    {
        LOG_PRINT("Variable found in global scope %p: %s [type: %s]\n", visitor->global_scope, node->variable_name, ast_type_to_string(global_variable_definition->variable_definition_value->type));
        return global_variable_definition->variable_definition_value;
    }

    log_error("Variable '%s' not defined\n", node->variable_name);
    exit(1);
}

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_int(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node)
{
    LOG_PRINT("Visiting compound\n");
    for (size_t i = 0; i < node->compound_size; i++)
    {
        AST_T *visited = visitor_visit(visitor, node->compound_value[i]);
        LOG_PRINT("Visited compound node %lu [type]: %s\n", i, ast_type_to_string(visited->type));
        if (visited->type == AST_RETURN)
        {
            LOG_PRINT("Return statement found in compound\n");
            return visited;
        }
    }

    return node;
}

AST_T *visitor_visit_function_call(visitor_T *visitor, AST_T *node)
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
        return init_ast(NOOP);
    }
    else if (strcmp(node->function_call_name, "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(node->function_call_name, "println") == 0)
    {
        builtin_println(visitor, node->function_call_arguments, node->function_call_arguments_size);
        return init_ast(NOOP);
    }
    else
    {
        AST_T *function_definition = NULL;
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

            size_t arguments_size = function_definition->function_definition_arguments_size;
            AST_T **arguments = calloc(arguments_size, sizeof(struct AST_STRUCT *));

            for (size_t i = 0; i < arguments_size; i++)
            {

                if (i >= node->function_call_arguments_size)
                {
                    log_error("Not enough arguments provided for function '%s' expected argument '%s'\n",
                              node->function_call_name, function_definition->function_definition_arguments[i]->variable_definition_variable_name);
                    exit(1);
                }

                LOG_PRINT("Passed argument type: %s\n", ast_type_to_string(node->function_call_arguments[i]->type));

                AST_T *argument_copy = calloc(1, sizeof(struct AST_STRUCT));
                AST_T *function_call_argument_copy = calloc(1, sizeof(struct AST_STRUCT));

                memcpy(argument_copy, function_definition->function_definition_arguments[i], sizeof(struct AST_STRUCT));
                memcpy(function_call_argument_copy, node->function_call_arguments[i], sizeof(struct AST_STRUCT));

                LOG_PRINT("Argument copy address: %p\n", argument_copy);
                LOG_PRINT("Function call argument copy address: %p\n", function_call_argument_copy);

                argument_copy->variable_definition_value = visitor_visit(visitor, function_call_argument_copy);

                arguments[i] = init_ast(AST_VARIABLE_DEFINITION);
                arguments[i]->variable_definition_variable_name = argument_copy->variable_name;
                arguments[i]->variable_definition_value = argument_copy->variable_definition_value;
                arguments[i]->variable_definition_variable_count = argument_copy->variable_definition_variable_count;
                LOG_PRINT("Adding argument %s [%s]\n",
                          arguments[i]->variable_definition_variable_name,
                          ast_type_to_string(arguments[i]->variable_definition_value->type));

                // free(argument_copy);
                // free(function_call_argument_copy);
            }

            visitor->scope_stack = push_scope_to_stack(visitor->scope_stack, init_scope());

            for (size_t i = 0; i < arguments_size; i++)
            {
                LOG_PRINT("Adding argument to scope: %s = %d\n", arguments[i]->variable_definition_variable_name, arguments[i]->variable_definition_value->int_value);

                visitor_add_variable_definition(visitor, arguments[i]);
            }

            LOG_PRINT("New scope after adding arguments\n");
            print_scope(visitor->scope_stack->scope);

            AST_T *function_body = visitor_visit(visitor, function_definition->function_definition_body);

            if (function_body->type == AST_RETURN)
            {
                LOG_PRINT("Visiting return value from function call\n");
                AST_T *result = visitor_visit(visitor, function_body->return_value);

                visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);
                return result;
            }

            visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);
            return function_definition;
        }
    }

    log_error("Function '%s' not defined\n", node->function_call_name);
    exit(1);
}

int visitor_get_node_value(visitor_T *visitor, AST_T *node)
{
    if (!node)
    {
        log_error("Node is NULL\n");
        exit(1);
    }

    LOG_PRINT("Getting value of node: %s\n", ast_type_to_string(node->type));

    switch (node->type)
    {
    case AST_INT:
        LOG_PRINT("Returning int value: %d\n", node->int_value);
        return node->int_value;
    case AST_VARIABLE:
        return visitor_get_node_value(visitor, visitor_visit_variable(visitor, node));
    case AST_FUNCTION_CALL:
        return visitor_get_node_value(visitor, visitor_visit_function_call(visitor, node));
    case AST_MUL:
    case AST_DIV:
    case AST_PLUS:
    case AST_MINUS:
    case AST_NESTED_EXPRESSION:
    case AST_RETURN:
    case AST_GT:
    case AST_LT:
    case AST_GTE:
    case AST_LTE:
    case AST_AND:
    case AST_OR:
    case AST_EQUAL:
        return visitor_get_node_value(visitor, visitor_visit_term(visitor, node));
    case AST_NOT:
        return visitor_get_node_value(visitor, visitor_visit_not(visitor, node));
    case AST_STRING:

        log_error("Cannot get value of a string node\n");
        exit(1);
    default:
        log_error("Unknown node type\n");
        exit(1);
    }
}

AST_T *visitor_visit_term(visitor_T *visitor, AST_T *node)
{
    LOG_PRINT("Visiting term %s\n\tleft: %s\n\tright: %s\n", ast_type_to_string(node->type), ast_type_to_string(node->left->type), ast_type_to_string(node->right->type));

    if (!node->left || !node->right)
    {
        return visitor_visit_factor(visitor, node);
    }

    AST_T *left = visitor_visit_factor(visitor, node->left);
    AST_T *right = visitor_visit_factor(visitor, node->right);

    if (left->type == AST_INT && right->type == AST_INT)
    {
        AST_T *result = init_ast(AST_INT);
        switch (node->type)
        {
        case AST_MUL:
            LOG_PRINT("Multiplying %d by %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value * right->int_value;
            break;
        case AST_DIV:
            LOG_PRINT("Dividing %d by %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value / right->int_value;
            break;
        case AST_PLUS:
            LOG_PRINT("Adding %d to %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value + right->int_value;
            break;
        case AST_MINUS:
            LOG_PRINT("Subtracting %d from %d\n", right->int_value, left->int_value);
            result->int_value = left->int_value - right->int_value;
            break;
        case AST_GT:
            LOG_PRINT("Comparing %d > %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value > right->int_value;
            break;
        case AST_LT:
            LOG_PRINT("Comparing %d < %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value < right->int_value;
            break;
        case AST_GTE:
            LOG_PRINT("Comparing %d >= %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value >= right->int_value;
            break;
        case AST_LTE:
            LOG_PRINT("Comparing %d <= %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value <= right->int_value;
            break;
        case AST_AND:
            LOG_PRINT("Comparing %d and %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value && right->int_value;
            break;
        case AST_OR:
            LOG_PRINT("Comparing %d or %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value || right->int_value;
            break;
        case AST_EQUAL:
            LOG_PRINT("Comparing %d == %d\n", left->int_value, right->int_value);
            result->int_value = left->int_value == right->int_value;
            break;
        default:
            log_error("Unknown operation: %s\n", ast_type_to_string(node->type));
            exit(1);
        }

        return result;
    }

    LOG_PRINT("Term: %s\n", ast_type_to_string(node->type));
    return init_ast(NOOP);
}

AST_T *visitor_visit_factor(visitor_T *visitor, AST_T *node)
{
    LOG_PRINT("Visiting factor %s\n", ast_type_to_string(node->type));
    switch (node->type)
    {
    case AST_INT:
        return node;
    case AST_VARIABLE:
        return visitor_visit_variable(visitor, node);
    case AST_FUNCTION_CALL:
        return visitor_visit_function_call(visitor, node);
    case AST_NESTED_EXPRESSION:
        return visitor_visit_term(visitor, node->nested_expression);
    case AST_PLUS:
    case AST_MINUS:
    case AST_MUL:
    case AST_DIV:
    case AST_GT:
    case AST_LT:
    case AST_GTE:
    case AST_LTE:
    case AST_EQUAL:
        return visitor_visit_term(visitor, node);
    case AST_NOT:
        return visitor_visit_not(visitor, node);
    default:
        log_error("Unknown node type: %s\n", ast_type_to_string(node->type));
        exit(1);
    }

    return node;
}

AST_T *visitor_visit_not(visitor_T *visitor, AST_T *node)
{
    LOG_PRINT("Visiting not\n");
    AST_T *factor = visitor_visit_factor(visitor, node->not_expression);
    if (factor->type == AST_INT)
    {
        factor->int_value = !factor->int_value;
        return factor;
    }
    log_error("Unsupported type for NOT operation: %s\n", ast_type_to_string(factor->type));
    exit(1);
}

AST_T *visitor_visit_if_branch(visitor_T *visitor, AST_T *node)
{
    LOG_PRINT("Visiting if branch\n");
    for (size_t i = 0; i < node->if_else_compound_size; i++)
    {
        AST_T *if_else = node->if_else_compound_value[i];

        if (if_else->type == AST_IF)
        {
            LOG_PRINT("Visiting if\n");
            if (visitor_get_node_value(visitor, if_else->if_condition))
            {
                LOG_PRINT("If condition true\n");
                return visitor_visit(visitor, if_else->if_body);
            }
        }
        else if (if_else->type == AST_ELSEIF)
        {
            LOG_PRINT("Visiting elseif\n");
            if (visitor_get_node_value(visitor, if_else->elseif_condition))
            {
                LOG_PRINT("Elseif condition true\n");
                return visitor_visit(visitor, if_else->elseif_body);
            }
        }
        else if (if_else->type == AST_ELSE)
        {
            LOG_PRINT("Visiting else\n");
            return visitor_visit(visitor, if_else->else_body);
        }
    }

    return init_ast(NOOP);
}

void visitor_add_variable_definition(visitor_T *visitor, AST_T *node)
{
    if (visitor->scope_stack->scope == (void *)0)
    {
        LOG_PRINT("Adding variable definition to global scope %p\n", visitor->global_scope);
        scope_add_variable_definition(visitor->global_scope, node);
    }
    else
    {
        LOG_PRINT("Adding variable definition to local scope %p\n", visitor->scope_stack->scope);
        scope_add_variable_definition(visitor->scope_stack->scope, node);
    }
}

void visitor_add_function_definition(visitor_T *visitor, AST_T *node)
{
    if (visitor->scope_stack->scope == (void *)0)
    {
        LOG_PRINT("Adding function definition to global scope %p\n", visitor->global_scope);
        scope_add_function_definition(visitor->global_scope, node);
    }
    else
    {
        LOG_PRINT("Adding function definition to local scope %p\n", visitor->scope_stack->scope);
        scope_add_function_definition(visitor->scope_stack->scope, node);
    }
}

static void builtin_print(visitor_T *visitor, AST_T **arguments, size_t arguments_size)
{
    for (size_t i = 0; i < arguments_size; i++)
    {
        if (!arguments[i])
        {
            log_error("Argument %lu is NULL\n", i);
            exit(1);
        }

        LOG_PRINT("Printing argument %lu with type: %s\n", i, ast_type_to_string(arguments[i]->type));
        AST_T *visited_ast = visitor_visit(visitor, arguments[i]);
        LOG_PRINT("Printing type: %s\n", ast_type_to_string(visited_ast->type));
        switch (visited_ast->type)
        {
        case AST_STRING:
            printf("%s", visited_ast->string_value);
            break;
        case AST_INT:
            printf("%d", visited_ast->int_value);
            break;
        case AST_VARIABLE_DEFINITION:
            LOG_PRINT("Variable definition value: %s\n", ast_type_to_string(visited_ast->variable_definition_value->type));
            printf("%s", visited_ast->variable_definition_value->string_value);
            break;
        default:
            log_error("Unsupported type for printing\n");
            exit(1);
        }
        if (i < arguments_size - 1)
        {
            printf(" ");
        }
    }
}

static void builtin_println(visitor_T *visitor, AST_T **arguments, size_t arguments_size)
{
    builtin_print(visitor, arguments, arguments_size);
    printf("\n");
}
