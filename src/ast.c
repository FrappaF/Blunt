#include "include/AST.h"
#include "include/logger.h"
#include <stdlib.h>
#include <stdio.h>

AST_T *init_ast(int type)
{
    AST_T *ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    /* AST_VARIABLE_DEFINITION */
    ast->variable_definition_variable_name = (void *)0;
    ast->variable_definition_value = (void *)0;

    /* AST_VARIABLE */
    ast->variable_name = (void *)0;

    /* AST_FUNCTION_CALL */
    ast->function_call_name = (void *)0;
    ast->function_call_arguments = (void *)0;
    ast->function_call_arguments_size = 0;

    /* AST_STRING */
    ast->string_value = (void *)0;

    /* AST_COMPOUND */
    ast->compound_value = (void *)0;
    ast->compound_size = 0;

    // LOG_PRINT("Initialized AST node of type: %d\n", type);
    return ast;
}

const char *ast_type_to_string(int type)
{
    switch (type)
    {
    case AST_VARIABLE_DEFINITION:
        return "AST_VARIABLE_DEFINITION";
    case AST_VARIABLE_COUNT:
        return "AST_VARIABLE_COUNT";
    case AST_VARIABLE:
        return "AST_VARIABLE";
    case AST_FUNCTION_DEFINITION:
        return "AST_FUNCTION_DEFINITION";
    case AST_FUNCTION_CALL:
        return "AST_FUNCTION_CALL";
    case AST_RETURN:
        return "AST_RETURN";
    case AST_STRING:
        return "AST_STRING";
    case AST_COMPOUND:
        return "AST_COMPOUND";
    case AST_INT:
        return "AST_INT";
    case AST_MUL:
        return "AST_MUL";
    case AST_DIV:
        return "AST_DIV";
    case AST_PLUS:
        return "AST_PLUS";
    case AST_MINUS:
        return "AST_MINUS";
    case AST_NESTED_EXPRESSION:
        return "AST_NESTED_EXPRESSION";
    case AST_IF_BRANCH:
        return "AST_IF_BRANCH";
    case AST_IF:
        return "AST_IF";
    case AST_ELSE:
        return "AST_ELSE";
    case AST_ELSEIF:
        return "AST_ELSEIF";
    case AST_GT:
        return "AST_GT";
    case AST_LT:
        return "AST_LT";
    case AST_GTE:
        return "AST_GTE";
    case AST_LTE:
        return "AST_LTE";
    case AST_AND:
        return "AST_AND";
    case AST_OR:
        return "AST_OR";
    case AST_NOT:
        return "AST_NOT";
    case AST_EQUAL:
        return "AST_EQUAL";
    case NOOP:
        return "NOOP";
    default:
        LOG_PRINT("Unknown AST type: %d\n", type);
        return "UNKNOWN";
    }
}

void ast_print(AST_T *node, int indent)
{
    if (!node)
        return;

    for (int i = 0; i < indent; i++)
        LOG_PRINT("  ");

    LOG_PRINT("%s\n", ast_type_to_string(node->type));

    switch (node->type)
    {
    case AST_VARIABLE_DEFINITION:
        for (int i = 0; i < indent + 1; i++)
            LOG_PRINT("  ");
        LOG_PRINT("Name: %s\n", node->variable_definition_variable_name);
        ast_print(node->variable_definition_value, indent + 1);
        break;
    case AST_VARIABLE:
        for (int i = 0; i < indent + 1; i++)
            LOG_PRINT("  ");
        LOG_PRINT("Name: %s\n", node->variable_name);
        break;
    case AST_FUNCTION_DEFINITION:
        for (int i = 0; i < indent + 1; i++)
            LOG_PRINT("  ");
        LOG_PRINT("Name: %s\n", node->function_definition_name);
        for (size_t i = 0; i < node->function_definition_arguments_size; i++)
            ast_print(node->function_definition_arguments[i], indent + 1);
        ast_print(node->function_definition_body, indent + 1);
        break;
    case AST_FUNCTION_CALL:
        for (int i = 0; i < indent + 1; i++)
            LOG_PRINT("  ");
        LOG_PRINT("Name: %s\n", node->function_call_name);
        for (size_t i = 0; i < node->function_call_arguments_size; i++)
            ast_print(node->function_call_arguments[i], indent + 1);
        break;
    case AST_RETURN:
        ast_print(node->return_value, indent + 1);
        break;
    case AST_STRING:
        for (int i = 0; i < indent + 1; i++)
            LOG_PRINT("  ");
        LOG_PRINT("Value: %s\n", node->string_value);
        break;
    case AST_INT:
        for (int i = 0; i < indent + 1; i++)
            LOG_PRINT("  ");
        LOG_PRINT("Value: %d\n", node->int_value);
        break;
    case AST_COMPOUND:
        for (size_t i = 0; i < node->compound_size; i++)
            ast_print(node->compound_value[i], indent + 1);
        break;
    case AST_IF_BRANCH:
        for (size_t i = 0; i < node->if_else_compound_size; i++)
            ast_print(node->if_else_compound_value[i], indent + 1);
    case AST_MUL:
    case AST_DIV:
    case AST_PLUS:
    case AST_MINUS:
    case AST_GT:
    case AST_LT:
    case AST_GTE:
    case AST_LTE:
    case AST_AND:
    case AST_OR:
    case AST_EQUAL:
        ast_print(node->left, indent + 1);
        ast_print(node->right, indent + 1);
        break;
    case AST_NOT:
        ast_print(node->not_expression, indent + 1);
        break;
    case AST_IF:
        ast_print(node->if_condition, indent + 1);
        ast_print(node->if_body, indent + 1);
        break;
    case AST_ELSE:
        ast_print(node->else_body, indent + 1);
        break;
    case AST_ELSEIF:
        ast_print(node->elseif_condition, indent + 1);
        ast_print(node->elseif_body, indent + 1);
        break;
    case AST_NESTED_EXPRESSION:
        ast_print(node->nested_expression, indent + 1);
        break;
    default:
        break;
    }
}