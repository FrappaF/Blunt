#include "include/visitor/visitor_expression.h"
#include "include/io/logger.h"
#include "include/scope/scope.h"
#include "include/visitor/visitor.h"
#include "include/ast/AST.h"
#include <stdio.h>
#include <string.h>

AST_T *visitor_visit_string(visitor_T *visitor, AST_STRING_T *node)
{
    return (AST_T *)node;
}

AST_T *visitor_visit_int(visitor_T *visitor, AST_INT_T *node)
{
    return (AST_T *)node;
}

AST_T *visitor_visit_array(visitor_T *visitor, AST_ARRAY_T *node)
{
    LOG_PRINT("Visiting array\n");

    for (size_t i = 0; i < node->array_size; i++)
    {
        LOG_PRINT("Visiting array element %lu\n", i);
        AST_T *array_value = visitor_visit(visitor, node->array_value[i]);
        node->array_value[i] = array_value;
    }

    return (AST_T *)node;
}

AST_T *visitor_visit_dot_expression(visitor_T *visitor, AST_DOT_EXPRESSION_T *node)
{
    LOG_PRINT("Visiting dot expression\n");

    if (!node->dot_expression_variable_name)
    {
        log_error("Dot expression variable name is NULL\n");
        exit(1);
    }

    AST_VARIABLE_T *variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
    variable->variable_name = node->dot_expression_variable_name;

    LOG_PRINT("Variable name: %s\n", node->dot_expression_variable_name);

    if (!node->dot_index)
    {
        log_error("Dot index is NULL\n");
        exit(1);
    }

    AST_T *dot_index = NULL;

    int is_assignment = node->dot_index->type == AST_VARIABLE_ASSIGNMENT;

    int is_function_call = node->dot_index->type == AST_FUNCTION_CALL;

    int is_dot_dot = node->dot_index->type == AST_DOT_DOT;

    if (is_assignment)
    {
        AST_VARIABLE_T *variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
        variable->variable_name = ((AST_VARIABLE_ASSIGNMENT_T *)node->dot_index)->variable_assignment_name;
        LOG_PRINT("Visiting %s\n", variable->variable_name);
        dot_index = visitor_visit(visitor, variable);
    }
    else if (is_function_call)
    {
        LOG_PRINT("Visiting function call from function definition\n");
        // Search for function definition for variable name
        AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->dot_expression_variable_name);
        if (!variable_definition)
        {
            log_error("Variable definition for %s not found\n", node->dot_expression_variable_name);
            exit(1);
        }

        if (variable_definition->variable_definition_value->type != AST_RUNTIME_FUNCTION_DEFINITION)
        {
            log_error("Variable definition for %s is not a function\n", node->dot_expression_variable_name);
            exit(1);
        }

        AST_RUNTIME_FUNCTION_DEFINITION_T *function_definition = (AST_RUNTIME_FUNCTION_DEFINITION_T *)(variable_definition->variable_definition_value);
        LOG_PRINT("Function definition variable size: %lu\n", function_definition->function_definition_variables_size);
        AST_FUNCTION_CALL_T *function_call = (AST_FUNCTION_CALL_T *)node->dot_index;

        return visitor_visit_runtime_function_call(visitor, function_definition, function_call);
    }
    else if (is_dot_dot)
    {
        dot_index = visitor_visit_last_variable(visitor, variable);
    }
    else
    {
        dot_index = visitor_visit(visitor, node->dot_index);
    }

    if (dot_index->type != AST_INT)
    {
        log_error("Dot index must be an integer\n");
        exit(1);
    }

    int index = ((AST_INT_T *)dot_index)->int_value;

    if (is_assignment)
    {
        AST_VARIABLE_ASSIGNMENT_T *variable_assignment = (AST_VARIABLE_ASSIGNMENT_T *)init_ast(AST_VARIABLE_ASSIGNMENT);

        // Create variable name in form 'name.index'
        char *variable_name = strcat(node->dot_expression_variable_name, ".");
        variable_name = strcat(variable_name, ((AST_VARIABLE_ASSIGNMENT_T *)node->dot_index)->variable_assignment_name);
        LOG_PRINT("Variable dot name: %s\n", variable_name);
        variable_assignment->variable_assignment_name = variable_name;
        variable_assignment->variable_assignment_value = ((AST_VARIABLE_ASSIGNMENT_T *)node->dot_index)->variable_assignment_value;

        return visitor_visit_variable_assignment_with_index(visitor, variable_assignment, index);
    }

    return visitor_visit_variable_with_index(visitor, variable, index);
}

AST_T *visitor_visit_dot_dot_expression(visitor_T *visitor, AST_DOT_DOT_EXPRESSION_T *node)
{
    LOG_PRINT("Visiting dot dot expression\n");

    if (!node->dot_dot_expression_variable_name)
    {
        log_error("Dot dot expression variable name is NULL\n");
        exit(1);
    }

    AST_VARIABLE_T *variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
    variable->variable_name = node->dot_dot_expression_variable_name;

    LOG_PRINT("Variable name: %s\n", node->dot_dot_expression_variable_name);

    if (!node->dot_dot_first_index)
    {
        log_error("Dot dot first index is NULL\n");
        exit(1);
    }

    if (!node->dot_dot_last_index)
    {
        log_error("Dot dot last index is NULL\n");
        exit(1);
    }

    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->dot_dot_expression_variable_name);

    if (!variable_definition)
    {
        log_error("Variable definition for %s not found\n", node->dot_dot_expression_variable_name);
        exit(1);
    }

    AST_T *dot_dot_first_index = visitor_visit(visitor, node->dot_dot_first_index);
    AST_T *dot_dot_last_index = visitor_visit(visitor, node->dot_dot_last_index);

    int first_index = -1;
    int last_index = -1;

    if (dot_dot_first_index->type == AST_INT)
    {
        first_index = ((AST_INT_T *)dot_dot_first_index)->int_value;
    }
    else if (dot_dot_first_index->type == AST_DOT_DOT)
    {
        first_index = 0;
    }
    else
    {
        log_error("Unsupported type for first index in dot dot notation\n");
        exit(1);
    }

    if (dot_dot_last_index->type == AST_INT)
    {
        last_index = ((AST_INT_T *)dot_dot_last_index)->int_value;
    }
    else if (dot_dot_last_index->type == AST_DOT_DOT)
    {
        last_index = builtin_len(visitor, (AST_T *)variable_definition->variable_definition_value) - 1;
    }
    else
    {
        log_error("Unsupported type for last index in dot dot notation\n");
        exit(1);
    }

    return visitor_visit_variable_with_dot_dot(visitor, variable, first_index, last_index);
}

AST_T *visitor_visit_dot_dot(visitor_T *visitor, AST_DOT_DOT_T *node)
{
    LOG_PRINT("Visiting dot dot\n");
    return (AST_T *)node;
}
