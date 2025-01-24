#include "../include/visitor/visitor.h"
#include "../include/io/logger.h"
#include "../include/token/token.h"
#include "../include/scope/scope.h"
#include "../include/ast/AST.h"
#include <stdio.h>
#include <string.h>

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

void builtin_print(visitor_T *visitor, AST_T **arguments, size_t arguments_size)
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

void builtin_println(visitor_T *visitor, AST_T **arguments, size_t arguments_size)
{
    builtin_print(visitor, arguments, arguments_size);
    printf("\n");
}

int builtin_len(visitor_T *visitor, AST_T *node)
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
