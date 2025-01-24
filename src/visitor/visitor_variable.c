#include "../include/visitor/visitor_variable.h"
#include "../include/io/logger.h"
#include "../include/scope/scope.h"
#include "../include/ast/AST.h"
#include <stdio.h>
#include <string.h>

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

AST_T *visitor_visit_variable(visitor_T *visitor, AST_VARIABLE_T *node)
{
    return visitor_visit_variable_with_index(visitor, node, -1);
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

AST_T *visitor_visit_variable_assignment(visitor_T *visitor, AST_VARIABLE_ASSIGNMENT_T *node)
{
    return visitor_visit_variable_assignment_with_index(visitor, node, -1);
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
