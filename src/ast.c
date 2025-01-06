#include "include/AST.h"
#include "include/logger.h"
#include <stdlib.h>
#include <stdio.h>

static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
        LOG_PRINT("  ");
}

size_t ast_get_size(AST_T *ast)
{
    size_t size = 1;

    switch (ast->type)
    {
    case AST_VARIABLE_DEFINITION:
        size = sizeof(AST_VARIABLE_DEFINITION_T);
        break;
    case AST_VARIABLE:
        size = sizeof(AST_VARIABLE_T);
        break;
    case AST_FUNCTION_DEFINITION:
        size = sizeof(AST_FUNCTION_DEFINITION_T);
        break;
    case AST_FUNCTION_CALL:
        size = sizeof(AST_FUNCTION_CALL_T);
        break;
    case AST_RETURN:
        size = sizeof(AST_RETURN_T);
        break;
    case AST_STRING:
        size = sizeof(AST_STRING_T);
        break;
    case AST_INT:
        size = sizeof(AST_INT_T);
        break;
    case AST_ARRAY:
        size = sizeof(AST_ARRAY_T);
        break;
    case AST_COMPOUND:
        size = sizeof(AST_COMPOUND_T);
        break;
    case AST_IF:
        size = sizeof(AST_IF_T);
        break;
    case AST_ELSE:
        size = sizeof(AST_ELSE_T);
        break;
    case AST_ELSEIF:
        size = sizeof(AST_ELSEIF_T);
        break;
    case AST_IF_ELSE_BRANCH:
        size = sizeof(AST_IF_ELSE_BRANCH_T);
        break;
    case AST_ADD_OP:
        size = sizeof(AST_ADD_OP_T);
        break;
    case AST_SUB_OP:
        size = sizeof(AST_SUB_OP_T);
        break;
    case AST_MUL_OP:
        size = sizeof(AST_MUL_OP_T);
        break;
    case AST_DIV_OP:
        size = sizeof(AST_DIV_OP_T);
        break;
    case AST_GT_OP:
        size = sizeof(AST_GT_OP_T);
        break;
    case AST_LT_OP:
        size = sizeof(AST_LT_OP_T);
        break;
    case AST_GTE_OP:
        size = sizeof(AST_GTE_OP_T);
        break;
    case AST_LTE_OP:
        size = sizeof(AST_LTE_OP_T);
        break;
    case AST_AND_OP:
        size = sizeof(AST_AND_OP_T);
        break;
    case AST_OR_OP:
        size = sizeof(AST_OR_OP_T);
        break;
    case AST_EQUAL_OP:
        size = sizeof(AST_EQUAL_OP_T);
        break;
    case AST_NOT:
        size = sizeof(AST_NOT_T);
        break;
    case AST_NESTED_EXPRESSION:
        size = sizeof(AST_NESTED_EXPRESSION_T);
        break;
    case AST_VARIABLE_COUNT:
        size = sizeof(AST_VARIABLE_COUNT_T);
        break;
    case AST_NOOP:
        size = sizeof(AST_T);
        break;
    case AST_VARIABLE_ASSIGNMENT:
        size = sizeof(AST_VARIABLE_ASSIGNMENT_T);
        break;
    default:
        size = sizeof(AST_T);
        break;
    }

    return size;
}

AST_T *init_ast(int type)
{
    AST_T *ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    switch (type)
    {
    case AST_VARIABLE_DEFINITION:
    {
        AST_VARIABLE_DEFINITION_T *variable_definition = calloc(1, sizeof(struct AST_VARIABLE_DEFINITION_STRUCT));
        variable_definition->base = *ast;
        variable_definition->variable_definition_variable_name = NULL;
        variable_definition->variable_definition_value = NULL;
        variable_definition->variable_definition_variable_count = NULL;
        return (AST_T *)variable_definition;
    }
    case AST_DOT_EXPRESSION:
    {
        AST_DOT_EXPRESSION_T *dot_expression = calloc(1, sizeof(struct AST_DOT_EXPRESSION_STRUCT));
        dot_expression->base = *ast;
        dot_expression->dot_expression_variable_name = NULL;
        dot_expression->dot_index = NULL;
        return (AST_T *)dot_expression;
    }
    case AST_VARIABLE:
    {
        AST_VARIABLE_T *variable = calloc(1, sizeof(struct AST_VARIABLE_STRUCT));
        variable->base = *ast;
        variable->variable_name = NULL;
        return (AST_T *)variable;
    }
    case AST_VARIABLE_ASSIGNMENT:
    {
        AST_VARIABLE_ASSIGNMENT_T *variable_assignment = calloc(1, sizeof(struct AST_VARIABLE_ASSIGNMENT_STRUCT));
        variable_assignment->base = *ast;
        variable_assignment->variable_assignment_name = NULL;
        variable_assignment->variable_assignment_value = NULL;
        return (AST_T *)variable_assignment;
    }
    case AST_FUNCTION_DEFINITION:
    {
        AST_FUNCTION_DEFINITION_T *function_definition = calloc(1, sizeof(struct AST_FUNCTION_DEFINITION_STRUCT));
        function_definition->base = *ast;
        function_definition->function_definition_name = NULL;
        function_definition->function_definition_arguments = NULL;
        function_definition->function_definition_arguments_size = 0;
        function_definition->function_definition_body = NULL;
        return (AST_T *)function_definition;
    }
    case AST_FUNCTION_CALL:
    {
        AST_FUNCTION_CALL_T *function_call = calloc(1, sizeof(struct AST_FUNCTION_CALL_STRUCT));
        function_call->base = *ast;
        function_call->function_call_name = NULL;
        function_call->function_call_arguments = NULL;
        function_call->function_call_arguments_size = 0;
        return (AST_T *)function_call;
    }
    case AST_RETURN:
    {
        AST_RETURN_T *return_node = calloc(1, sizeof(struct AST_RETURN_STRUCT));
        return_node->base = *ast;
        return_node->return_value = NULL;
        return (AST_T *)return_node;
    }
    case AST_STRING:
    {
        AST_STRING_T *string_node = calloc(1, sizeof(struct AST_STRING_STRUCT));
        string_node->base = *ast;
        string_node->string_value = NULL;
        return (AST_T *)string_node;
    }
    case AST_INT:
    {
        AST_INT_T *int_node = calloc(1, sizeof(struct AST_INT_STRUCT));
        int_node->base = *ast;
        int_node->int_value = 0;
        return (AST_T *)int_node;
    }
    case AST_ARRAY:
    {
        AST_ARRAY_T *array_node = calloc(1, sizeof(struct AST_ARRAY_STRUCT));
        array_node->base = *ast;
        array_node->array_value = NULL;
        array_node->array_size = 0;
        return (AST_T *)array_node;
    }
    case AST_COMPOUND:
    {
        AST_COMPOUND_T *compound_node = calloc(1, sizeof(struct AST_COMPOUND_STRUCT));
        compound_node->base = *ast;
        compound_node->compound_value = NULL;
        compound_node->compound_size = 0;
        return (AST_T *)compound_node;
    }
    case AST_IF:
    {
        AST_IF_T *if_branch_node = calloc(1, sizeof(struct AST_IF_STRUCT));
        if_branch_node->base = *ast;
        if_branch_node->if_body = NULL;
        if_branch_node->if_condition = NULL;
        return (AST_T *)if_branch_node;
    }
    case AST_ELSE:
    {
        AST_ELSE_T *else_node = calloc(1, sizeof(struct AST_ELSE_STRUCT));
        else_node->base = *ast;
        else_node->else_body = NULL;
        return (AST_T *)else_node;
    }
    case AST_ELSEIF:
    {
        AST_ELSEIF_T *elseif_node = calloc(1, sizeof(struct AST_ELSEIF_STRUCT));
        elseif_node->base = *ast;
        elseif_node->elseif_body = NULL;
        elseif_node->elseif_condition = NULL;
        return (AST_T *)elseif_node;
    }
    case AST_IF_ELSE_BRANCH:
    {
        AST_IF_ELSE_BRANCH_T *if_else_branch_node = calloc(1, sizeof(struct AST_IF_ELSE_BRANCH_STRUCT));
        if_else_branch_node->base = *ast;
        if_else_branch_node->if_else_compound_value = NULL;
        if_else_branch_node->if_else_compound_size = 0;
        return (AST_T *)if_else_branch_node;
    }
    case AST_ADD_OP:
    {
        AST_ADD_OP_T *add_node = calloc(1, sizeof(struct AST_ADD_OP_STRUCT));
        add_node->base = *ast;
        add_node->left = NULL;
        add_node->right = NULL;
        return (AST_T *)add_node;
    }
    case AST_SUB_OP:
    {
        AST_SUB_OP_T *sub_node = calloc(1, sizeof(struct AST_SUB_OP_STRUCT));
        sub_node->base = *ast;
        sub_node->left = NULL;
        sub_node->right = NULL;
        return (AST_T *)sub_node;
    }
    case AST_MUL_OP:
    {
        AST_MUL_OP_T *mul_node = calloc(1, sizeof(struct AST_MUL_OP_STRUCT));
        mul_node->base = *ast;
        mul_node->left = NULL;
        mul_node->right = NULL;
        return (AST_T *)mul_node;
    }
    case AST_DIV_OP:
    {
        AST_DIV_OP_T *div_node = calloc(1, sizeof(struct AST_DIV_OP_STRUCT));
        div_node->base = *ast;
        div_node->left = NULL;
        div_node->right = NULL;
        return (AST_T *)div_node;
    }
    case AST_GT_OP:
    {
        AST_GT_OP_T *gt_node = calloc(1, sizeof(struct AST_GT_OP_STRUCT));
        gt_node->base = *ast;
        gt_node->left = NULL;
        gt_node->right = NULL;
        return (AST_T *)gt_node;
    }
    case AST_LT_OP:
    {
        AST_LT_OP_T *lt_node = calloc(1, sizeof(struct AST_LT_OP_STRUCT));
        lt_node->base = *ast;
        lt_node->left = NULL;
        lt_node->right = NULL;
        return (AST_T *)lt_node;
    }
    case AST_GTE_OP:
    {
        AST_GTE_OP_T *gte_node = calloc(1, sizeof(struct AST_GTE_OP_STRUCT));
        gte_node->base = *ast;
        gte_node->left = NULL;
        gte_node->right = NULL;
        return (AST_T *)gte_node;
    }
    case AST_LTE_OP:
    {
        AST_LTE_OP_T *lte_node = calloc(1, sizeof(struct AST_LTE_OP_STRUCT));
        lte_node->base = *ast;
        lte_node->left = NULL;
        lte_node->right = NULL;
        return (AST_T *)lte_node;
    }

    case AST_AND_OP:
    {
        AST_AND_OP_T *and_node = calloc(1, sizeof(struct AST_AND_OP_STRUCT));
        and_node->base = *ast;
        and_node->left = NULL;
        and_node->right = NULL;
        return (AST_T *)and_node;
    }
    case AST_OR_OP:
    {
        AST_OR_OP_T *or_node = calloc(1, sizeof(struct AST_OR_OP_STRUCT));
        or_node->base = *ast;
        or_node->left = NULL;
        or_node->right = NULL;
        return (AST_T *)or_node;
    }
    case AST_EQUAL_OP:
    {
        AST_EQUAL_OP_T *equal_node = calloc(1, sizeof(struct AST_EQUAL_OP_STRUCT));
        equal_node->base = *ast;
        equal_node->left = NULL;
        equal_node->right = NULL;
        return (AST_T *)equal_node;
    }
    case AST_NOT:
    {
        AST_NOT_T *not_node = calloc(1, sizeof(struct AST_NOT_STRUCT));
        not_node->base = *ast;
        not_node->not_expression = NULL;
        return (AST_T *)not_node;
    }
    case AST_NESTED_EXPRESSION:
    {
        AST_NESTED_EXPRESSION_T *nested_expression_node = calloc(1, sizeof(struct AST_NESTED_EXPRESSION_STRUCT));
        nested_expression_node->base = *ast;
        nested_expression_node->nested_expression = NULL;
        return (AST_T *)nested_expression_node;
    }
    case AST_VARIABLE_COUNT:
    {
        AST_VARIABLE_COUNT_T *variable_count_node = calloc(1, sizeof(struct AST_VARIABLE_COUNT_STRUCT));
        variable_count_node->base = *ast;
        variable_count_node->variable_count_value = 0;
        return (AST_T *)variable_count_node;
    }
    case AST_NOOP:
    {
        return ast;
    }
    default:
        return ast;
    }
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
    case AST_ARRAY:
        return "AST_ARRAY";
    case AST_ADD_OP:
        return "AST_ADD_OP";
    case AST_SUB_OP:
        return "AST_SUB_OP";
    case AST_MUL_OP:
        return "AST_MUL_OP";
    case AST_DIV_OP:
        return "AST_DIV_OP";
    case AST_GT_OP:
        return "AST_GT_OP";
    case AST_LT_OP:
        return "AST_LT_OP";
    case AST_GTE_OP:
        return "AST_GTE_OP";
    case AST_LTE_OP:
        return "AST_LTE_OP";
    case AST_AND_OP:
        return "AST_AND_OP";
    case AST_OR_OP:
        return "AST_OR_OP";
    case AST_EQUAL_OP:
        return "AST_EQUAL_OP";
    case AST_NOT:
        return "AST_NOT";
    case AST_NESTED_EXPRESSION:
        return "AST_NESTED_EXPRESSION";
    case AST_IF_ELSE_BRANCH:
        return "AST_IF_BRANCH";
    case AST_IF:
        return "AST_IF";
    case AST_ELSE:
        return "AST_ELSE";
    case AST_ELSEIF:
        return "AST_ELSEIF";
    case AST_VARIABLE_ASSIGNMENT:
        return "AST_VARIABLE_ASSIGNMENT";
    case AST_DOT_EXPRESSION:
        return "AST_DOT_EXPRESSION";
    case AST_NOOP:
        return "AST_NOOP";
    default:
        LOG_PRINT("Unknown AST type: %d\n", type);
        return "UNKNOWN";
    }
}

void ast_print(AST_T *node, int indent)
{
    if (!node)
        return;

    print_indent(indent);
    LOG_PRINT("%s\n", ast_type_to_string(node->type));

    switch (node->type)
    {
    case AST_VARIABLE_DEFINITION:
        print_indent(indent + 1);
        LOG_PRINT("Name: %s\n", ((AST_VARIABLE_DEFINITION_T *)node)->variable_definition_variable_name);
        ast_print(((AST_VARIABLE_DEFINITION_T *)node)->variable_definition_value, indent + 1);
        break;
    case AST_VARIABLE:
        print_indent(indent + 1);
        LOG_PRINT("Name: %s\n", ((AST_VARIABLE_T *)node)->variable_name);
        break;
    case AST_FUNCTION_DEFINITION:
        print_indent(indent + 1);
        LOG_PRINT("Name: %s\n", ((AST_FUNCTION_DEFINITION_T *)node)->function_definition_name);
        for (size_t i = 0; i < ((AST_FUNCTION_DEFINITION_T *)node)->function_definition_arguments_size; i++)
            ast_print(((AST_FUNCTION_DEFINITION_T *)node)->function_definition_arguments[i], indent + 1);
        ast_print(((AST_FUNCTION_DEFINITION_T *)node)->function_definition_body, indent + 1);
        break;
    case AST_FUNCTION_CALL:
        print_indent(indent + 1);
        LOG_PRINT("Name: %s\n", ((AST_FUNCTION_CALL_T *)node)->function_call_name);
        for (size_t i = 0; i < ((AST_FUNCTION_CALL_T *)node)->function_call_arguments_size; i++)
            ast_print(((AST_FUNCTION_CALL_T *)node)->function_call_arguments[i], indent + 1);
        break;
    case AST_RETURN:
        ast_print(((AST_RETURN_T *)node)->return_value, indent + 1);
        break;
    case AST_STRING:
        print_indent(indent + 1);
        LOG_PRINT("Value: %s\n", ((AST_STRING_T *)node)->string_value);
        break;
    case AST_INT:
        print_indent(indent + 1);
        LOG_PRINT("Value: %d\n", ((AST_INT_T *)node)->int_value);
        break;
    case AST_ARRAY:
        for (size_t i = 0; i < ((AST_ARRAY_T *)node)->array_size; i++)
            ast_print(((AST_ARRAY_T *)node)->array_value[i], indent + 1);
        break;
    case AST_VARIABLE_ASSIGNMENT:
        print_indent(indent + 1);
        LOG_PRINT("Name: %s\n", ((AST_VARIABLE_ASSIGNMENT_T *)node)->variable_assignment_name);
        ast_print(((AST_VARIABLE_ASSIGNMENT_T *)node)->variable_assignment_value, indent + 1);
        break;
    case AST_COMPOUND:
        for (size_t i = 0; i < ((AST_COMPOUND_T *)node)->compound_size; i++)
            ast_print(((AST_COMPOUND_T *)node)->compound_value[i], indent + 1);
        break;
    case AST_IF_ELSE_BRANCH:
        for (size_t i = 0; i < ((AST_IF_ELSE_BRANCH_T *)node)->if_else_compound_size; i++)
            ast_print(((AST_IF_ELSE_BRANCH_T *)node)->if_else_compound_value[i], indent + 1);
        break;
    case AST_ADD_OP:
    case AST_SUB_OP:
    case AST_MUL_OP:
    case AST_DIV_OP:
    case AST_GT_OP:
    case AST_LT_OP:
    case AST_GTE_OP:
    case AST_LTE_OP:
    case AST_AND_OP:
    case AST_OR_OP:
    case AST_EQUAL_OP:
        ast_print(((AST_ADD_OP_T *)node)->left, indent + 1);
        ast_print(((AST_ADD_OP_T *)node)->right, indent + 1);
        break;
    case AST_NOT:
        ast_print(((AST_NOT_T *)node)->not_expression, indent + 1);
        break;
    case AST_IF:
        ast_print(((AST_IF_T *)node)->if_condition, indent + 1);
        ast_print(((AST_IF_T *)node)->if_body, indent + 1);
        break;
    case AST_ELSE:
        ast_print(((AST_ELSE_T *)node)->else_body, indent + 1);
        break;
    case AST_ELSEIF:
        ast_print(((AST_ELSEIF_T *)node)->elseif_condition, indent + 1);
        ast_print(((AST_ELSEIF_T *)node)->elseif_body, indent + 1);
        break;
    case AST_NESTED_EXPRESSION:
        ast_print(((AST_NESTED_EXPRESSION_T *)node)->nested_expression, indent + 1);
        break;
    case AST_DOT_EXPRESSION:
        print_indent(indent + 1);
        LOG_PRINT("Name: %s\n", ((AST_DOT_EXPRESSION_T *)node)->dot_expression_variable_name);
        ast_print(((AST_DOT_EXPRESSION_T *)node)->dot_index, indent + 1);
        break;
    default:
        break;
    }
}