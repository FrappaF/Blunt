#include "include/visitor.h"
#include "include/logger.h"
#include "include/token.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>
#include "include/ast.h"

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
        return visitor_visit_variable_definition(visitor, (AST_VARIABLE_DEFINITION_T *)node);
    case AST_VARIABLE:
        return visitor_visit_variable(visitor, (AST_VARIABLE_T *)node);
    case AST_STRING:
        return visitor_visit_string(visitor, (AST_STRING_T *)node);
    case AST_INT:
        return visitor_visit_int(visitor, (AST_INT_T *)node);
    case AST_COMPOUND:
        return visitor_visit_compound(visitor, (AST_COMPOUND_T *)node);
    case AST_FUNCTION_CALL:
        return visitor_visit_function_call(visitor, (AST_FUNCTION_CALL_T *)node);
    case AST_FUNCTION_DEFINITION:
        return visitor_visit_function_definition(visitor, (AST_FUNCTION_DEFINITION_T *)node);
    case AST_RETURN:
        return visitor_visit_return(visitor, (AST_RETURN_T *)node);
    case AST_IF_ELSE_BRANCH:
        return visitor_visit_if_branch(visitor, (AST_IF_ELSE_BRANCH_T *)node);
    case AST_MUL_OP:
    case AST_DIV_OP:
    case AST_ADD_OP:
    case AST_SUB_OP:
    case AST_NESTED_EXPRESSION:
        return visitor_visit_term(visitor, node);
    default:
        LOG_PRINT("Node of type: [%s] not supported\n", ast_type_to_string(node->type));
        return init_ast(AST_NOOP);
    }
}

AST_T *visitor_visit_return(visitor_T *visitor, AST_RETURN_T *node)
{
    if (!node->return_value)
    {
        log_error("Return value is NULL\n");
        exit(1);
    }

    LOG_PRINT("Visiting return [type]: %s\n", ast_type_to_string(node->return_value->type));
    return (AST_T *)node;
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

        AST_VARIABLE_DEFINITION_T *argument = node->function_definition_arguments[i];
        if (!argument)
        {
            log_error("Function definition argument %lu is NULL\n", i);
            exit(1);
        }

        LOG_PRINT("Function argument %lu [%s]\n", i, ast_type_to_string(argument->base.type));

        if (!argument->variable_definition_variable_name)
        {
            log_error("Function definition argument name is NULL\n");
            exit(1);
        }

        LOG_PRINT("Function argument %lu: %s\n", i, argument->variable_definition_variable_name);
    }

    visitor_add_function_definition(visitor, (AST_T *)node);

    return (AST_T *)node;
}

AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_VARIABLE_DEFINITION_T *node)
{
    if (!node->variable_definition_variable_name)
    {
        log_error("Variable definition name is NULL\n");
        exit(1);
    }

    LOG_PRINT("Visiting variable definition\n");
    LOG_PRINT("Variable name: %s\n", node->variable_definition_variable_name);

    if (node->variable_definition_variable_count != NULL)
    {
        AST_VARIABLE_COUNT_T *variable_count = (AST_VARIABLE_COUNT_T *)node->variable_definition_variable_count;
        LOG_PRINT("Variable count: %d\n", variable_count->variable_count_value);
    }

    if (!node->variable_definition_value)
    {
        log_error("Variable definition value is NULL\n");
        exit(1);
    }

    AST_T *variable_value = visitor_visit(visitor, node->variable_definition_value);
    node->variable_definition_value = variable_value;

    visitor_add_variable_definition(visitor, (AST_T *)node);

    return (AST_T *)node;
}

AST_T *visitor_visit_variable(visitor_T *visitor, AST_VARIABLE_T *node)
{
    if (!node->variable_name)
    {
        log_error("Variable name is NULL\n");
        exit(1);
    }

    scope_stack_T *current_scope_stack = visitor->scope_stack;
    while (current_scope_stack)
    {

        AST_VARIABLE_DEFINITION_T *local_variable_definition = scope_get_variable_definition(current_scope_stack->scope, node->variable_name);
        if (local_variable_definition)
        {
            LOG_PRINT("Local variable found in scope %p: %s [type: %s]\n", current_scope_stack->scope, node->variable_name, ast_type_to_string(local_variable_definition->variable_definition_value->type));
            return local_variable_definition->variable_definition_value;
        }
        current_scope_stack = current_scope_stack->parent;
    }

    AST_VARIABLE_DEFINITION_T *global_variable_definition = scope_get_variable_definition(visitor->global_scope, node->variable_name);
    if (global_variable_definition)
    {
        LOG_PRINT("Variable found in global scope %p: %s [type: %s]\n", visitor->global_scope, node->variable_name, ast_type_to_string(global_variable_definition->variable_definition_value->type));
        return global_variable_definition->variable_definition_value;
    }

    log_error("Variable '%s' not defined\n", node->variable_name);
    exit(1);
}

AST_T *visitor_visit_string(visitor_T *visitor, AST_STRING_T *node)
{
    return (AST_T *)node;
}

AST_T *visitor_visit_int(visitor_T *visitor, AST_INT_T *node)
{
    return (AST_T *)node;
}

AST_T *visitor_visit_compound(visitor_T *visitor, AST_COMPOUND_T *node)
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

    return (AST_T *)node;
}

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

            size_t arguments_size = function_definition->function_definition_arguments_size;
            AST_T **arguments = calloc(arguments_size, sizeof(struct AST_VARIABLE_DEFINITION_T *));

            for (size_t i = 0; i < arguments_size; i++)
            {
                AST_VARIABLE_DEFINITION_T *argument_copy = calloc(1, sizeof(struct AST_VARIABLE_DEFINITION_STRUCT));
                if (!argument_copy)
                {
                    log_error("Failed to allocate memory for argument_copy\n");
                    exit(1);
                }

                AST_VARIABLE_DEFINITION_T *original_argument = function_definition->function_definition_arguments[i];

                if (i >= node->function_call_arguments_size)
                {
                    log_error("Not enough arguments provided for function '%s' expected argument '%s'\n",
                              node->function_call_name,
                              original_argument->variable_definition_variable_name);
                    exit(1);
                }

                LOG_PRINT("Passed argument type: %s\n", ast_type_to_string(node->function_call_arguments[i]->type));

                argument_copy->variable_definition_variable_name = strdup(original_argument->variable_definition_variable_name);
                argument_copy->variable_definition_value = visitor_visit(visitor, node->function_call_arguments[i]);
                argument_copy->variable_definition_variable_count = original_argument->variable_definition_variable_count;

                arguments[i] = argument_copy;

                LOG_PRINT("Adding argument %s [%s]\n",
                          argument_copy->variable_definition_variable_name,
                          ast_type_to_string(argument_copy->variable_definition_value->type));
            }

            visitor->scope_stack = push_scope_to_stack(visitor->scope_stack, init_scope());

            for (size_t i = 0; i < arguments_size; i++)
            {
                LOG_PRINT("Adding argument to scope: %s\n",
                          ((AST_VARIABLE_DEFINITION_T *)arguments[i])->variable_definition_variable_name);

                visitor_add_variable_definition(visitor, ((AST_VARIABLE_DEFINITION_T *)arguments[i]));
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
        LOG_PRINT("Returning int value: %d\n", ((AST_INT_T *)node)->int_value);
        return ((AST_INT_T *)node)->int_value;
    case AST_VARIABLE:
        return visitor_get_node_value(visitor, visitor_visit_variable(visitor, (AST_VARIABLE_T *)node));
    case AST_FUNCTION_CALL:
        return visitor_get_node_value(visitor, visitor_visit_function_call(visitor, (AST_FUNCTION_CALL_T *)node));
    case AST_MUL_OP:
    case AST_DIV_OP:
    case AST_ADD_OP:
    case AST_SUB_OP:
    case AST_NESTED_EXPRESSION:
    case AST_RETURN:
    case AST_GT_OP:
    case AST_LT_OP:
    case AST_GTE_OP:
    case AST_LTE_OP:
    case AST_AND_OP:
    case AST_OR_OP:
    case AST_EQUAL_OP:
        return visitor_get_node_value(visitor, visitor_visit_term(visitor, node));
    case AST_NOT:
        return visitor_get_node_value(visitor, visitor_visit_not(visitor, (AST_NOT_T *)node));
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
    // Use add operation as a base
    AST_ADD_OP_T *op = (AST_ADD_OP_T *)node;

    LOG_PRINT("Visiting term %s\n\tleft: %s\n\tright: %s\n",
              ast_type_to_string(node->type),
              ast_type_to_string(op->left->type),
              ast_type_to_string(op->right->type));

    if (!op->left || !op->right)
    {
        return visitor_visit_factor(visitor, node);
    }

    AST_T *left = visitor_visit_factor(visitor, op->left);
    AST_T *right = visitor_visit_factor(visitor, op->right);

    if (left->type == AST_INT && right->type == AST_INT)
    {
        AST_INT_T *left_int = (AST_INT_T *)left;
        AST_INT_T *right_int = (AST_INT_T *)right;

        AST_INT_T *result = (AST_INT_T *)init_ast(AST_INT);
        switch (node->type)
        {
        case AST_MUL_OP:
            LOG_PRINT("Multiplying %d by %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value * right_int->int_value;
            break;
        case AST_DIV_OP:
            LOG_PRINT("Dividing %d by %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value / right_int->int_value;
            break;
        case AST_ADD_OP:
            LOG_PRINT("Adding %d to %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value + right_int->int_value;
            break;
        case AST_SUB_OP:
            LOG_PRINT("Subtracting %d from %d\n", right_int->int_value, left_int->int_value);
            result->int_value = left_int->int_value - right_int->int_value;
            break;
        case AST_GT_OP:
            LOG_PRINT("Comparing %d > %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value > right_int->int_value;
            break;
        case AST_LT_OP:
            LOG_PRINT("Comparing %d < %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value < right_int->int_value;
            break;
        case AST_GTE_OP:
            LOG_PRINT("Comparing %d >= %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value >= right_int->int_value;
            break;
        case AST_LTE_OP:
            LOG_PRINT("Comparing %d <= %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value <= right_int->int_value;
            break;
        case AST_AND_OP:
            LOG_PRINT("Comparing %d and %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value && right_int->int_value;
            break;
        case AST_OR_OP:
            LOG_PRINT("Comparing %d or %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value || right_int->int_value;
            break;
        case AST_EQUAL_OP:
            LOG_PRINT("Comparing %d == %d\n", left_int->int_value, right_int->int_value);
            result->int_value = left_int->int_value == right_int->int_value;
            break;
        default:
            log_error("Unknown operation: %s\n", ast_type_to_string(node->type));
            exit(1);
        }

        return result;
    }

    LOG_PRINT("Term: %s\n", ast_type_to_string(node->type));
    return init_ast(AST_NOOP);
}

AST_T *visitor_visit_factor(visitor_T *visitor, AST_T *node)
{
    LOG_PRINT("Visiting factor %s\n", ast_type_to_string(node->type));
    switch (node->type)
    {
    case AST_INT:
        return node;
    case AST_VARIABLE:
        return visitor_visit_variable(visitor, (AST_VARIABLE_T *)node);
    case AST_FUNCTION_CALL:
        return visitor_visit_function_call(visitor, (AST_FUNCTION_CALL_T *)node);
    case AST_NESTED_EXPRESSION:
        return visitor_visit_term(visitor, ((AST_NESTED_EXPRESSION_T *)node)->nested_expression);
    case AST_ADD_OP:
    case AST_SUB_OP:
    case AST_MUL_OP:
    case AST_DIV_OP:
    case AST_GT_OP:
    case AST_LT_OP:
    case AST_GTE_OP:
    case AST_LTE_OP:
    case AST_EQUAL_OP:
        return visitor_visit_term(visitor, node);
    case AST_NOT:
        return visitor_visit_not(visitor, (AST_NOT_T *)node);
    default:
        log_error("Unknown node type: %s\n", ast_type_to_string(node->type));
        exit(1);
    }

    return node;
}

AST_T *visitor_visit_not(visitor_T *visitor, AST_NOT_T *node)
{
    LOG_PRINT("Visiting not\n");
    AST_T *factor = visitor_visit_factor(visitor, node->not_expression);
    if (factor->type == AST_INT)
    {
        AST_INT_T *int_factor = (AST_INT_T *)factor;
        int_factor->int_value = !int_factor->int_value;
        return factor;
    }
    log_error("Unsupported type for NOT operation: %s\n", ast_type_to_string(factor->type));
    exit(1);
}

AST_T *visitor_visit_if_branch(visitor_T *visitor, AST_IF_ELSE_BRANCH_T *node)
{
    LOG_PRINT("Visiting if branch\n");
    for (size_t i = 0; i < node->if_else_compound_size; i++)
    {
        AST_T *if_else = node->if_else_compound_value[i];

        if (if_else->type == AST_IF)
        {
            AST_IF_T *if_branch = (AST_IF_T *)if_else;
            LOG_PRINT("Visiting if\n");
            if (visitor_get_node_value(visitor, if_branch->if_condition))
            {
                LOG_PRINT("If condition true\n");
                return visitor_visit(visitor, if_branch->if_body);
            }
        }
        else if (if_else->type == AST_ELSEIF)
        {
            AST_ELSEIF_T *if_else_branch = (AST_ELSEIF_T *)if_else;
            LOG_PRINT("Visiting elseif\n");
            if (visitor_get_node_value(visitor, if_else_branch->elseif_condition))
            {
                LOG_PRINT("Elseif condition true\n");
                return visitor_visit(visitor, if_else_branch->elseif_body);
            }
        }
        else if (if_else->type == AST_ELSE)
        {
            AST_ELSE_T *else_branch = (AST_ELSE_T *)if_else;

            LOG_PRINT("Visiting else\n");
            return visitor_visit(visitor, else_branch->else_body);
        }
    }

    return init_ast(AST_NOOP);
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
            printf("%s", ((AST_STRING_T *)visited_ast)->string_value);
            break;
        case AST_INT:
            printf("%d", ((AST_INT_T *)visited_ast)->int_value);
            break;
        case AST_VARIABLE_DEFINITION:
            LOG_PRINT("Variable definition value: %s\n", ast_type_to_string(((AST_VARIABLE_DEFINITION_T *)visited_ast)->variable_definition_value->type));
            builtin_print(visitor, (AST_T **)&((AST_VARIABLE_DEFINITION_T *)visited_ast)->variable_definition_value, 1);
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