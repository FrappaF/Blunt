#include "include/visitor/visitor_statement.h"
#include "include/io/logger.h"
#include "include/scope/scope.h"
#include "include/ast/AST.h"
#include <stdio.h>
#include <string.h>

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

AST_T *visitor_visit_for_loop(visitor_T *visitor, AST_FOR_LOOP_T *node)
{
    LOG_PRINT("Visiting for loop\n");

    if (!node->for_loop_increment)
    {
        log_error("For loop increment is NULL\n");
        exit(1);
    }

    if (node->for_loop_increment->type != AST_VARIABLE)
    {
        log_error("For loop increment must be a variable\n");
        exit(1);
    }

    AST_VARIABLE_DEFINITION_T *for_variable_definition = visitor_get_variable_definition(
        visitor,
        ((AST_VARIABLE_T *)node->for_loop_variable)->variable_name);

    if (!for_variable_definition)
    {
        log_error("For loop variable definition not found\n");
        exit(1);
    }

    // Search for the increment variable definition
    AST_VARIABLE_DEFINITION_T *increment_variable_definition = visitor_get_variable_definition(
        visitor,
        ((AST_VARIABLE_T *)node->for_loop_increment)->variable_name);

    // Push a new scope for the for loop
    visitor->scope_stack = push_scope_to_stack(visitor->scope_stack, init_scope());

    if (!increment_variable_definition)
    {
        LOG_PRINT("Increment variable definition not found, creating one\n");
        AST_VARIABLE_T *increment_variable = (AST_VARIABLE_T *)node->for_loop_increment;
        increment_variable_definition = (AST_VARIABLE_DEFINITION_T *)init_ast(AST_VARIABLE_DEFINITION);
        // Variable name
        increment_variable_definition->variable_definition_variable_name = increment_variable->variable_name;
        // Variable value
        increment_variable_definition->variable_definition_value = (AST_T *)init_ast(AST_INT);
        ((AST_INT_T *)increment_variable_definition->variable_definition_value)->int_value = 0;

        // Variable count
        increment_variable_definition->variable_definition_variable_count = (AST_T *)init_ast(AST_VARIABLE_COUNT);
        ((AST_VARIABLE_COUNT_T *)increment_variable_definition->variable_definition_variable_count)->variable_count_value = 1;

        // Add the variable definition to the scope
        visitor_add_variable_definition(visitor, (AST_T *)increment_variable_definition);
    }

    if (increment_variable_definition->variable_definition_value->type != AST_INT)
    {
        log_error("Increment variable must be an integer\n");
        exit(1);
    }

    if (!node->for_loop_condition)
    {
        // Creating default condition i < var count
        LOG_PRINT("For loop condition is NULL, creating default one\n");
        AST_LT_OP_T *condition = (AST_LT_OP_T *)init_ast(AST_LT_OP);
        condition->left = node->for_loop_increment;

        AST_VARIABLE_COUNT_T *variable_count = for_variable_definition->variable_definition_variable_count;
        AST_INT_T *int_value = (AST_INT_T *)init_ast(AST_INT);
        int_value->int_value = variable_count->variable_count_value;
        condition->right = (AST_T *)int_value;

        node->for_loop_condition = (AST_T *)condition;
    }

    while (visitor_get_node_value(visitor, node->for_loop_condition))
    {
        visitor_visit(visitor, node->for_loop_body);

        // Increment by one the value of increment variable
        ((AST_INT_T *)increment_variable_definition->variable_definition_value)->int_value++;
    }

    // Pop the scope for the for loop
    visitor->scope_stack = pop_scope_from_stack(visitor->scope_stack);

    return init_ast(AST_NOOP);
}

AST_T *visitor_visit_save(visitor_T *visitor, AST_SAVE_T *node)
{
    LOG_PRINT("Visiting save\n");

    if (!visitor->current_function)
    {
        log_error("Save must be called inside a function definition\n");
        exit(1);
    }

    AST_VARIABLE_T *save_variable = node->save_value;

    // Search for the variable definition
    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(
        visitor,
        save_variable->variable_name);

    if (!variable_definition)
    {
        log_error("Variable definition for %s not found\n", save_variable->variable_name);
        exit(1);
    }

    // Get the value of the variable
    AST_T *variable_value = variable_definition->variable_definition_value;

    LOG_PRINT("Variable value type: %s\n", ast_type_to_string(variable_value->type));
    AST_VARIABLE_COUNT_T *variable_count = (AST_VARIABLE_COUNT_T *)variable_definition->variable_definition_variable_count;
    // Create a new variable definition
    AST_VARIABLE_DEFINITION_T *save_variable_definition = (AST_VARIABLE_DEFINITION_T *)init_ast(AST_VARIABLE_DEFINITION);
    save_variable_definition->variable_definition_variable_name = save_variable->variable_name;
    save_variable_definition->variable_definition_value = variable_value;
    LOG_PRINT("variable count: %lu\n", variable_count->variable_count_value);
    save_variable_definition->variable_definition_variable_count = (AST_VARIABLE_COUNT_T *)variable_definition->variable_definition_variable_count;

    LOG_PRINT("Adding variable definition for %s (%s) to function definition\n", save_variable->variable_name, ast_type_to_string(variable_value->type));
    // Add the variable to the function definition variables
    // Realloc
    visitor->current_function->function_definition_variables_size++;
    visitor->current_function->function_definition_variables = realloc(
        visitor->current_function->function_definition_variables,
        (visitor->current_function->function_definition_variables_size + 1) * sizeof(struct AST_VARIABLE_DEFINITION_STRUCT *));
    // Add the variable to the end of the array
    visitor->current_function->function_definition_variables[visitor->current_function->function_definition_variables_size - 1] = save_variable_definition;

    return init_ast(AST_NOOP);
}

AST_T *visitor_visit_term(visitor_T *visitor, AST_T *node)
{
    if (node->type == AST_NESTED_EXPRESSION)
    {
        return visitor_visit_factor(visitor, node);
    }
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

    if (left->type == AST_STRING && right->type == AST_STRING)
    {
        AST_STRING_T *left_string = (AST_STRING_T *)left;
        AST_STRING_T *right_string = (AST_STRING_T *)right;

        AST_STRING_T *result = (AST_STRING_T *)init_ast(AST_STRING);
        switch (node->type)
        {
        case AST_ADD_OP:
            LOG_PRINT("Concatenating %s with %s\n", left_string->string_value, right_string->string_value);
            result->string_value = calloc(strlen(left_string->string_value) + strlen(right_string->string_value) + 1, sizeof(char));
            strcat(result->string_value, left_string->string_value);
            strcat(result->string_value, right_string->string_value);
            break;
        default:
            log_error("Unknown operation for strings: %s\n", ast_type_to_string(node->type));
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
        return visitor_visit_int(visitor, (AST_INT_T *)node);
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
    case AST_DOT_EXPRESSION:
        return visitor_visit_dot_expression(visitor, (AST_DOT_EXPRESSION_T *)node);
    case AST_DOT_DOT_EXPRESSION:
        return visitor_visit_dot_dot_expression(visitor, (AST_DOT_DOT_EXPRESSION_T *)node);
    case AST_STRING:
        return visitor_visit_string(visitor, (AST_STRING_T *)node);
    default:
        log_error("Unknown node type: %s\n", ast_type_to_string(node->type));
        exit(1);
    }

    return node;
}
