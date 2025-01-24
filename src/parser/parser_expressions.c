#include "../include/parser/parser_expressions.h"
#include "../include/io/logger.h"
#include <string.h>
#include <stdio.h>

// Function to get the precedence of a token type
int get_precedence(int token_type)
{
    switch (token_type)
    {
    case TOKEN_NOT:
        return 6;
    case TOKEN_EQUAL:
        return 7;
    case TOKEN_OR:
        return 1;
    case TOKEN_AND:
        return 2;
    case TOKEN_GT:
    case TOKEN_LT:
    case TOKEN_GTE:
    case TOKEN_LTE:
        return 3;
    case TOKEN_PLUS:
    case TOKEN_MINUS:
        return 4;
    case TOKEN_MUL:
    case TOKEN_DIV:
        return 5;
    default:
        return -1;
    }
}

// Parses an expression based on the current token type.
AST_T *parser_parse_expression(parser_T *parser)
{
    return parser_parse_expression_with_precedence(parser, 0);
}

// Parses a factor based on the current token type.
AST_T *parser_parse_factor(parser_T *parser)
{
    return parser_parse_factor_with_precedence(parser, 0);
}

// Parses a function call.
AST_T *parser_parse_function_call(parser_T *parser)
{
    char *function_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_LPAREN);

    AST_FUNCTION_CALL_T *ast_function_call = (AST_FUNCTION_CALL_T *)init_ast(AST_FUNCTION_CALL);
    ast_function_call->function_call_name = function_name;
    ast_function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT *));

    while (parser->current_token->type != TOKEN_RPAREN)
    {
        AST_T *ast_expression = parser_parse_expression(parser);

        int last_idx = ++ast_function_call->function_call_arguments_size;
        ast_function_call->function_call_arguments = realloc(
            ast_function_call->function_call_arguments,
            (last_idx + 1) * sizeof(struct AST_STRUCT *));

        ast_function_call->function_call_arguments[last_idx - 1] = ast_expression;

        if (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);
        }
    }

    parser_eat(parser, TOKEN_RPAREN);

    return (AST_T *)ast_function_call;
}

// Parses a string literal.
AST_T *parser_parse_string(parser_T *parser)
{
    char *value = parser->current_token->value;
    parser_eat(parser, TOKEN_STRING);

    AST_STRING_T *ast_string = (AST_STRING_T *)init_ast(AST_STRING);
    ast_string->string_value = value;

    return (AST_T *)ast_string;
}

// Parses a variable or function call.
AST_T *parser_parse_variable(parser_T *parser)
{
    char *token_name = parser->current_token->value;
    LOG_PRINT("Parsing variable: %s\n", token_name);

    // Check if the variable is an int
    if (atoi(token_name) != 0 || strcmp(token_name, "0") == 0)
    {
        AST_INT_T *ast_int = (AST_INT_T *)init_ast(AST_INT);
        ast_int->int_value = atoi(token_name);
        parser_eat(parser, TOKEN_INT);
        return (AST_T *)ast_int;
    }

    if (parser->peek_token->type == TOKEN_LPAREN)
    {
        parser_eat(parser, TOKEN_ID);
        return parser_parse_function_call(parser);
    }

    if (parser->current_token->type == TOKEN_STRING)
    {
        return parser_parse_string(parser);
    }

    AST_VARIABLE_T *ast_variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_name;

    parser_eat(parser, parser->current_token->type);

    return (AST_T *)ast_variable;
}

// Parses a dot expression.
AST_T *parser_parse_dot_expression(parser_T *parser)
{
    LOG_PRINT("Parsing dot expression\n");
    char *variable_name = parser->prev_token->value;

    parser_eat(parser, TOKEN_DOT);

    char *dot_index_chars = parser->current_token->value;

    AST_T *ast_dot_expression = parser_parse_expression_with_precedence(parser, 20);

    AST_DOT_EXPRESSION_T *ast_dot_expression_node = (AST_DOT_EXPRESSION_T *)init_ast(AST_DOT_EXPRESSION);
    ast_dot_expression_node->dot_expression_variable_name = variable_name;
    ast_dot_expression_node->dot_index = ast_dot_expression;

    if (parser->current_token->type == TOKEN_EQUALS)
    {
        parser_eat(parser, TOKEN_EQUALS);
        AST_VARIABLE_ASSIGNMENT_T *ast_variable_assignment = (AST_VARIABLE_ASSIGNMENT_T *)init_ast(AST_VARIABLE_ASSIGNMENT);
        ast_variable_assignment->variable_assignment_name = strcat(variable_name, ".");
        ast_variable_assignment->variable_assignment_name = strcat(ast_variable_assignment->variable_assignment_name, dot_index_chars);
        ast_variable_assignment->variable_assignment_value = parser_parse_expression(parser);

        return (AST_T *)ast_variable_assignment;
    }

    // Dot dot annotation (.3.7)
    if (parser->current_token->type == TOKEN_DOT)
    {
        /** Will be AST_DOT_DOT_EXPRESSION(
         *  variable_name,
         *  first index (ast_dot_expression),
         *  last index (new_ast_dot_expression)
         */

        parser_eat(parser, TOKEN_DOT);
        AST_DOT_DOT_EXPRESSION_T *ast_dot_dot_expression = (AST_DOT_DOT_EXPRESSION_T *)init_ast(AST_DOT_DOT_EXPRESSION);
        ast_dot_dot_expression->dot_dot_expression_variable_name = variable_name;
        ast_dot_dot_expression->dot_dot_first_index = ast_dot_expression;
        ast_dot_dot_expression->dot_dot_last_index = parser_parse_expression_with_precedence(parser, 20);

        return (AST_T *)ast_dot_dot_expression;
    }

    return (AST_T *)ast_dot_expression_node;
}

// Parses an identifier, which could be a variable or a function call.
AST_T *parser_parse_id(parser_T *parser)
{
    LOG_PRINT("Parsing id: %s\n", parser->current_token->value);
    char *token_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_LPAREN)
    {
        return parser_parse_function_call(parser);
    }
    if (parser->current_token->type == TOKEN_EQUALS)
    {
        return parser_parse_variable_assignment(parser);
    }
    if (parser->current_token->type == TOKEN_LSQUARE)
    {
        return parser_parse_array(parser);
    }
    if (parser->current_token->type == TOKEN_DOT)
    {
        return parser_parse_dot_expression(parser);
    }

    AST_VARIABLE_T *ast_variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_name;

    return (AST_T *)ast_variable;
}

// Parses a variable count statement.
AST_T *parser_parse_variable_count(parser_T *parser)
{
    LOG_PRINT("Parsing variable count\n");

    AST_VARIABLE_COUNT_T *ast_variable_count = (AST_VARIABLE_COUNT_T *)init_ast(AST_VARIABLE_COUNT);
    int count = atoi(parser->current_token->value);

    if (count <= 0)
    {
        fprintf(stderr, "Variable count must be greater than 0\n");
        exit(1);
    }

    ast_variable_count->variable_count_value = count;

    parser_eat(parser, TOKEN_INT);

    return (AST_T *)ast_variable_count;
}

// Parses a variable assignment statement.
AST_T *parser_parse_variable_assignment(parser_T *parser)
{
    LOG_PRINT("Parsing variable assignment\n");
    char *variable_name = parser->prev_token->value;

    parser_eat(parser, TOKEN_EQUALS);

    AST_T *ast_variable_assignment_value = parser_parse_expression(parser);

    AST_VARIABLE_ASSIGNMENT_T *ast_variable_assignment = (AST_VARIABLE_ASSIGNMENT_T *)init_ast(AST_VARIABLE_ASSIGNMENT);
    ast_variable_assignment->variable_assignment_name = variable_name;
    ast_variable_assignment->variable_assignment_value = ast_variable_assignment_value;

    return (AST_T *)ast_variable_assignment;
}

// Parses an integer literal.
AST_T *parser_parse_int(parser_T *parser)
{
    int value = atoi(parser->current_token->value);
    parser_eat(parser, TOKEN_INT);

    AST_INT_T *ast_int = (AST_INT_T *)init_ast(AST_INT);
    ast_int->int_value = value;

    return (AST_T *)ast_int;
}

// Parses an array.
AST_T *parser_parse_array(parser_T *parser)
{
    LOG_PRINT("Parsing array\n");
    parser_eat(parser, TOKEN_LSQUARE);

    AST_ARRAY_T *ast_array = (AST_ARRAY_T *)init_ast(AST_ARRAY);

    while (parser->current_token->type != TOKEN_RSQUARE)
    {
        AST_T *ast_expression = parser_parse_expression(parser);

        int last_idx = ++ast_array->array_size;

        // size_t expression_size = ast_get_size(ast_expression);

        ast_array->array_value = realloc(
            ast_array->array_value,
            (last_idx + 1) * sizeof(struct AST_STRUCT *));

        ast_array->array_value[last_idx - 1] = ast_expression;

        if (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);
        }
    }

    parser_eat(parser, TOKEN_RSQUARE);

    return (AST_T *)ast_array;
}

// Parses a function argument.
AST_T *parser_parse_function_argument(parser_T *parser)
{
    LOG_PRINT("Parsing function argument\n");
    char *variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    AST_VARIABLE_T *ast_variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
    ast_variable->variable_name = variable_name;

    return (AST_T *)ast_variable;
}

// Parses an expression with precedence.
AST_T *parser_parse_expression_with_precedence(parser_T *parser, int precedence)
{
    LOG_PRINT("Parsing expression %s with precedence %d\n", token_type_to_string(parser->current_token->type), precedence);
    AST_T *left = parser_parse_factor_with_precedence(parser, precedence);

    while (parser->current_token->type != TOKEN_EOF && get_precedence(parser->current_token->type) > precedence)
    {
        int token_type = parser->current_token->type;
        int token_precedence = get_precedence(token_type);
        parser_eat(parser, token_type);
        AST_T *right = parser_parse_expression_with_precedence(parser, token_precedence);
        LOG_PRINT("Parsed right expression: %s\n", ast_type_to_string(right->type));
        int type;
        switch (token_type)
        {
        case TOKEN_PLUS:
            type = AST_ADD_OP;
            AST_ADD_OP_T *add_node = (AST_ADD_OP_T *)init_ast(AST_ADD_OP);
            add_node->left = left;
            add_node->right = right;
            left = (AST_T *)add_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(add_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(add_node->right->type));
            break;
        case TOKEN_MINUS:
            type = AST_SUB_OP;
            AST_SUB_OP_T *sub_node = (AST_SUB_OP_T *)init_ast(AST_SUB_OP);
            sub_node->left = left;
            sub_node->right = right;
            left = (AST_T *)sub_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(sub_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(sub_node->right->type));
            break;
        case TOKEN_GT:
            type = AST_GT_OP;
            AST_GT_OP_T *gt_node = (AST_GT_OP_T *)init_ast(AST_GT_OP);
            gt_node->left = left;
            gt_node->right = right;
            left = (AST_T *)gt_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(gt_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(gt_node->right->type));
            break;
        case TOKEN_LT:
            type = AST_LT_OP;
            AST_LT_OP_T *lt_node = (AST_LT_OP_T *)init_ast(AST_LT_OP);
            lt_node->left = left;
            lt_node->right = right;
            left = (AST_T *)lt_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(lt_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(lt_node->right->type));
            break;
        case TOKEN_GTE:
            type = AST_GTE_OP;
            AST_GTE_OP_T *gte_node = (AST_GTE_OP_T *)init_ast(AST_GTE_OP);
            gte_node->left = left;
            gte_node->right = right;
            left = (AST_T *)gte_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(gte_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(gte_node->right->type));
            break;
        case TOKEN_LTE:
            type = AST_LTE_OP;
            AST_LTE_OP_T *lte_node = (AST_LTE_OP_T *)init_ast(AST_LTE_OP);
            lte_node->left = left;
            lte_node->right = right;
            left = (AST_T *)lte_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(lte_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(lte_node->right->type));
            break;
        case TOKEN_AND:
            type = AST_AND_OP;
            AST_AND_OP_T *and_node = (AST_AND_OP_T *)init_ast(AST_AND_OP);
            and_node->left = left;
            and_node->right = right;
            left = (AST_T *)and_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(and_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(and_node->right->type));
            break;
        case TOKEN_OR:
            type = AST_OR_OP;
            AST_OR_OP_T *or_node = (AST_OR_OP_T *)init_ast(AST_OR_OP);
            or_node->left = left;
            or_node->right = right;
            left = (AST_T *)or_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(or_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(or_node->right->type));
            break;
        case TOKEN_EQUAL:
            type = AST_EQUAL_OP;
            AST_EQUAL_OP_T *equal_node = (AST_EQUAL_OP_T *)init_ast(AST_EQUAL_OP);
            equal_node->left = left;
            equal_node->right = right;
            left = (AST_T *)equal_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(equal_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(equal_node->right->type));
            break;
        case TOKEN_MUL:
            type = AST_MUL_OP;
            AST_MUL_OP_T *mul_node = (AST_MUL_OP_T *)init_ast(AST_MUL_OP);
            mul_node->left = left;
            mul_node->right = right;
            left = (AST_T *)mul_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(mul_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(mul_node->right->type));
            break;
        case TOKEN_DIV:
            type = AST_DIV_OP;
            AST_DIV_OP_T *div_node = (AST_DIV_OP_T *)init_ast(AST_DIV_OP);
            div_node->left = left;
            div_node->right = right;
            left = (AST_T *)div_node;
            LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
            LOG_PRINT("\tLeft: %s\n", ast_type_to_string(div_node->left->type));
            LOG_PRINT("\tRight: %s\n", ast_type_to_string(div_node->right->type));
            break;
        default:
            log_error("Unknown token type: %s\n", token_type_to_string(token_type));
            exit(1);
        }
    }
    return left;
}

// Parses a factor with precedence.
AST_T *parser_parse_factor_with_precedence(parser_T *parser, int precedence)
{
    LOG_PRINT("Parsing factor %s [%s] with precedence %d\n", parser->current_token->value, token_type_to_string(parser->current_token->type), precedence);
    AST_T *node = NULL;

    if (parser->current_token->type == TOKEN_INT)
    {
        node = parser_parse_int(parser);
    }
    else if (parser->current_token->type == TOKEN_ID)
    {
        node = parser_parse_id(parser);
    }
    else if (parser->current_token->type == TOKEN_STRING)
    {
        node = parser_parse_string(parser);
    }
    else if (parser->current_token->type == TOKEN_LSQUARE)
    {
        node = parser_parse_array(parser);
    }
    else if (parser->current_token->type == TOKEN_LPAREN)
    {
        parser_eat(parser, TOKEN_LPAREN);
        node = (AST_NESTED_EXPRESSION_T *)init_ast(AST_NESTED_EXPRESSION);
        ((AST_NESTED_EXPRESSION_T *)node)->nested_expression = parser_parse_expression_with_precedence(parser, 0);
        parser_eat(parser, TOKEN_RPAREN);
    }
    else if (parser->current_token->type == TOKEN_NOT)
    {
        parser_eat(parser, TOKEN_NOT);
        node = (AST_NOT_T *)init_ast(AST_NOT);
        ((AST_NOT_T *)node)->not_expression = parser_parse_factor_with_precedence(parser, get_precedence(TOKEN_NOT));
    }
    else if (parser->current_token->type == TOKEN_DOT)
    {
        parser_eat(parser, TOKEN_DOT);
        node = (AST_DOT_DOT_T *)init_ast(AST_DOT_DOT);
    }
    else
    {
        fprintf(stderr, "Unexpected token: %s\n", token_type_to_string(parser->current_token->type));
        exit(1);
    }
    return node;
}
