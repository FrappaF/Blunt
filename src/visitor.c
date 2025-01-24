#include "include/visitor.h"
#include "include/logger.h"
#include "include/token.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>
#include "include/AST.h"

// Function declarations
static void builtin_print(visitor_T *visitor, AST_T **arguments, size_t arguments_size);
static void builtin_println(visitor_T *visitor, AST_T **arguments, size_t arguments_size);
static int builtin_len(visitor_T *visitor, AST_T *string);
void visitor_add_function_definition(visitor_T *visitor, AST_T *node);
void visitor_add_variable_definition(visitor_T *visitor, AST_T *node);

int visitor_get_variable_count(visitor_T *visitor, AST_VARIABLE_T *node);

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
    visitor->current_function = NULL;

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
    case AST_VARIABLE_ASSIGNMENT:
        return visitor_visit_variable_assignment(visitor, (AST_VARIABLE_ASSIGNMENT_T *)node);
    case AST_DOT_EXPRESSION:
        return visitor_visit_dot_expression(visitor, (AST_DOT_EXPRESSION_T *)node);
    case AST_DOT_DOT_EXPRESSION:
        return visitor_visit_dot_dot_expression(visitor, (AST_DOT_DOT_EXPRESSION_T *)node);
    case AST_ARRAY:
        return visitor_visit_array(visitor, (AST_ARRAY_T *)node);
    case AST_MUL_OP:
    case AST_DIV_OP:
    case AST_ADD_OP:
    case AST_SUB_OP:
    case AST_NESTED_EXPRESSION:
        return visitor_visit_term(visitor, node);
    case AST_FOR_LOOP:
        return visitor_visit_for_loop(visitor, (AST_FOR_LOOP_T *)node);
    case AST_SAVE:
        return visitor_visit_save(visitor, (AST_SAVE_T *)node);

    case AST_DOT_DOT:
        return visitor_visit_dot_dot(visitor, (AST_DOT_DOT_T *)node);
    default:
        LOG_PRINT("Node of type: [%s] not supported\n", ast_type_to_string(node->type));
        return init_ast(AST_NOOP);
    }
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
        // Seach for function definition for variable name
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

AST_T *visitor_visit_variable_with_dot_dot(visitor_T *visitor, AST_VARIABLE_T *node, int first_index, int last_index)
{
    LOG_PRINT("Visiting variable with dot dot\n");

    if (first_index > last_index)
    {
        log_error("First index must be less than last index\n");
        exit(1);
    }

    if (first_index < 0 || last_index < 0)
    {
        log_error("First index and last index must be greater than or equal to 0\n");
        exit(1);
    }

    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->variable_name);

    int length_of_variable = builtin_len(visitor, (AST_T *)variable_definition->variable_definition_value);

    if (last_index > length_of_variable)
    {
        log_error("Last index must be less than the length of the variable\n");
        exit(1);
    }
    if (first_index > length_of_variable)
    {
        log_error("First index must be less than the length of the variable\n");
        exit(1);
    }

    switch (variable_definition->variable_definition_value->type)
    {
    case AST_ARRAY:
    {
        // Create a new array with the values from the first index to the last index excluded
        AST_ARRAY_T *array = (AST_ARRAY_T *)variable_definition->variable_definition_value;
        AST_ARRAY_T *new_array = (AST_ARRAY_T *)init_ast(AST_ARRAY);
        new_array->array_size = last_index - first_index;
        new_array->array_value = calloc(new_array->array_size, sizeof(struct AST_STRUCT *));
        for (size_t i = 0; i < new_array->array_size; i++)
        {
            new_array->array_value[i] = array->array_value[first_index + i];
        }
        return (AST_T *)new_array;
    }
    case AST_STRING:
    {
        // Create a new string with the values from the first index to the last index
        AST_STRING_T *string = (AST_STRING_T *)variable_definition->variable_definition_value;
        int length = last_index - first_index;
        char *new_string = calloc(length + 1, sizeof(char));
        for (size_t i = 0; i < length; i++)
        {
            new_string[i] = string->string_value[first_index + i];
        }
        new_string[length] = '\0';
        AST_STRING_T *new_string_node = (AST_STRING_T *)init_ast(AST_STRING);
        new_string_node->string_value = new_string;
        return (AST_T *)new_string_node;
    }
    default:
    {
        log_error("Variable definition value must be an array or string\n");
        exit(1);
    }
    }
}

AST_T *visitor_visit_dot_dot(visitor_T *visitor, AST_DOT_DOT_T *node)
{
    LOG_PRINT("Visiting dot dot\n");
    return (AST_T *)node;
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

    LOG_PRINT("Variable value type: %s\n", ast_type_to_string(variable_value->type));

    visitor_add_variable_definition(visitor, (AST_T *)node);

    return (AST_T *)node;
}

AST_T *visitor_visit_last_variable(visitor_T *visitor, AST_VARIABLE_T *node)
{
    LOG_PRINT("Visiting last variable\n");

    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->variable_name);

    if (!variable_definition)
    {
        log_error("Variable '%s' not defined\n", node->variable_name);
        exit(1);
    }

    if (variable_definition->variable_definition_value->type == AST_ARRAY)
    {
        AST_ARRAY_T *array = (AST_ARRAY_T *)variable_definition->variable_definition_value;
        return array->array_value[array->array_size - 1];
    }
    else if (variable_definition->variable_definition_value->type == AST_STRING)
    {
        AST_STRING_T *string = (AST_STRING_T *)variable_definition->variable_definition_value;
        AST_STRING_T *last_char = (AST_STRING_T *)init_ast(AST_STRING);
        last_char->string_value = &(string->string_value)[strlen(string->string_value) - 1];
        return (AST_T *)last_char;
    }
    else
    {
        return variable_definition->variable_definition_value;
    }
}

AST_T *visitor_visit_variable_with_index(visitor_T *visitor, AST_VARIABLE_T *node, int index)
{

    LOG_PRINT("Visiting variable with index %d\n", index);

    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->variable_name);

    if (!variable_definition)
    {
        log_error("Variable '%s' not defined\n", node->variable_name);
        exit(1);
    }

    LOG_PRINT("Variable found: %s [type: %s]\n", node->variable_name, ast_type_to_string(variable_definition->variable_definition_value->type));

    if (index == -1)
    {
        return variable_definition->variable_definition_value;
    }

    if (variable_definition->variable_definition_variable_count <= index)
    {
        log_error("Index out of bounds\n");
        exit(1);
    }

    if (variable_definition->variable_definition_value->type == AST_ARRAY)
    {
        AST_ARRAY_T *array = (AST_ARRAY_T *)variable_definition->variable_definition_value;
        return array->array_value[index];
    }

    return variable_definition->variable_definition_value;
}

AST_VARIABLE_DEFINITION_T *visitor_get_variable_definition(visitor_T *visitor, char *variable_name)
{
    scope_stack_T *current_scope_stack = visitor->scope_stack;
    while (current_scope_stack)
    {
        AST_VARIABLE_DEFINITION_T *variable_definition = scope_get_variable_definition(current_scope_stack->scope, variable_name);
        if (variable_definition)
        {
            return variable_definition;
        }
        current_scope_stack = current_scope_stack->parent;
    }

    return scope_get_variable_definition(visitor->global_scope, variable_name);
}

AST_T *visitor_visit_variable_assignment_with_index(visitor_T *visitor, AST_VARIABLE_ASSIGNMENT_T *node, int index)
{
    LOG_PRINT("Visiting variable assignment for %s with index %d\n", node->variable_assignment_name, index);

    if (!node->variable_assignment_name)
    {
        log_error("Variable assignment name is NULL\n");
        exit(1);
    }

    if (!node->variable_assignment_value)
    {
        log_error("Variable assignment value is NULL\n");
        exit(1);
    }

    // Check if name is in form 'name.index'
    char *dot = strchr(node->variable_assignment_name, '.');
    if (dot)
    {
        char *variable_name = strtok(node->variable_assignment_name, ".");
        char *indexName = strtok(NULL, ".");
        if (!variable_name || !indexName)
        {
            log_error("Invalid dot expression\n");
            exit(1);
        }
        LOG_PRINT("Variable name: %s\tindex: %s\n", variable_name, indexName);

        AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, variable_name);
        if (!variable_definition)
        {
            log_error("Variable '%s' not defined\n", variable_name);
            exit(1);
        }

        int index = atoi(indexName);
        // If index is not an integer search for variable with that name
        if (index == 0 && strcmp(indexName, "0") != 0)
        {
            AST_VARIABLE_DEFINITION_T *index_definition = visitor_get_variable_definition(visitor, indexName);
            if (!index_definition)
            {
                log_error("Index '%s' not defined\n", index);
                exit(1);
            }
            AST_T *index_value = visitor_visit(visitor, index_definition->variable_definition_value);
            if (index_value->type != AST_INT)
            {
                log_error("Index must be an integer\n");
                exit(1);
            }
            index = ((AST_INT_T *)index_value)->int_value;
        }

        if (index >= variable_definition->variable_definition_variable_count || index < 0)
        {
            log_error("Index out of bounds\n");
            exit(1);
        }

        if (variable_definition->variable_definition_value->type == AST_ARRAY)
        {
            LOG_PRINT("Variable is an array, changing the index element\n");
            AST_ARRAY_T *array = (AST_ARRAY_T *)variable_definition->variable_definition_value;
            if (array->array_size <= index)
            {
                log_error("Index out of bounds\n");
                exit(1);
            }
            array->array_value[index] = visitor_visit(visitor, node->variable_assignment_value);
            return (AST_T *)array->array_value[index];
        }
        else
        {
            LOG_PRINT("Variable is not an array, changing the value to array\n");
            AST_T *old_value = variable_definition->variable_definition_value;
            variable_definition->variable_definition_value = (AST_ARRAY_T *)init_ast(AST_ARRAY);
            AST_ARRAY_T *array = (AST_ARRAY_T *)variable_definition->variable_definition_value;
            array->array_size = ((AST_VARIABLE_COUNT_T *)variable_definition->variable_definition_variable_count)->variable_count_value;
            array->array_value = calloc(array->array_size, sizeof(struct AST_STRUCT *));
            for (int i = 0; i < array->array_size; i++)
            {
                if (i == index)
                {
                    LOG_PRINT("Setting index %d to new value\n", i);
                    array->array_value[i] = visitor_visit(visitor, node->variable_assignment_value);
                }
                else
                {
                    LOG_PRINT("Copying old value to index %d\n", i);
                    array->array_value[i] = old_value;
                }
            }

            return (AST_T *)array->array_value[index];
        }
    }

    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->variable_assignment_name);
    if (!variable_definition)
    {
        log_error("Variable '%s' not defined\n", node->variable_assignment_name);
        exit(1);
    }

    if (index == -1)
    {
        variable_definition->variable_definition_value = visitor_visit(visitor, node->variable_assignment_value);
        return variable_definition->variable_definition_value;
    }

    if (variable_definition->variable_definition_variable_count <= index)
    {
        log_error("Index out of bounds\n");
        exit(1);
    }

    if (variable_definition->variable_definition_value->type == AST_ARRAY)
    {
        AST_ARRAY_T *array = (AST_ARRAY_T *)variable_definition->variable_definition_value;
        array->array_value[index] = visitor_visit(visitor, node->variable_assignment_value);
        return array->array_value[index];
    }

    return visitor_visit_variable_assignment(visitor, node);
}

AST_T *visitor_visit_variable_assignment(visitor_T *visitor, AST_VARIABLE_ASSIGNMENT_T *node)
{
    return visitor_visit_variable_assignment_with_index(visitor, node, -1);
}

AST_T *visitor_visit_variable(visitor_T *visitor, AST_VARIABLE_T *node)
{
    return visitor_visit_variable_with_index(visitor, node, -1);
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
    else if (strcmp(node->function_call_name, "len") == 0)
    {
        int lentgh = builtin_len(visitor, node->function_call_arguments[0]);
        AST_INT_T *result = (AST_INT_T *)init_ast(AST_INT);
        result->int_value = lentgh;
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

    // Search for the incerement variable definition
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

int visitor_get_variable_count(visitor_T *visitor, AST_VARIABLE_T *node)
{
    AST_VARIABLE_DEFINITION_T *variable_definition = visitor_get_variable_definition(visitor, node->variable_name);
    if (!variable_definition)
    {
        log_error("Variable '%s' not defined\n", node->variable_name);
        exit(1);
    }

    AST_VARIABLE_COUNT_T *variable_count = (AST_VARIABLE_COUNT_T *)(variable_definition->variable_definition_variable_count);

    return variable_count->variable_count_value;
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
        case AST_ARRAY:
            LOG_PRINT("Array size: %lu\n", ((AST_ARRAY_T *)visited_ast)->array_size);
            for (size_t j = 0; j < ((AST_ARRAY_T *)visited_ast)->array_size; j++)
            {
                builtin_print(visitor, (AST_T **)&((AST_ARRAY_T *)visited_ast)->array_value[j], 1);
                if (j < ((AST_ARRAY_T *)visited_ast)->array_size - 1)
                {
                    printf(" ");
                }
            }
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

static int builtin_len(visitor_T *visitor, AST_T *node)
{

    if (node->type == AST_ARRAY)
    {
        AST_ARRAY_T *array = (AST_ARRAY_T *)node;
        return array->array_size;
    }

    AST_STRING_T *string = NULL;
    if (node->type == AST_STRING)
    {
        string = (AST_STRING_T *)node;
        return strlen(string->string_value) + 1;
    }

    AST_T *visited_ast = visitor_visit(visitor, node);
    if (visited_ast->type == AST_STRING)
    {
        string = (AST_STRING_T *)visited_ast;
        return strlen(string->string_value) + 1;
    }

    if (visited_ast->type == AST_ARRAY)
    {
        AST_ARRAY_T *array = (AST_ARRAY_T *)visited_ast;
        return array->array_size;
    }

    log_error("Unsupported type for len\n");
    exit(1);
}
