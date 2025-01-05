#include "include/parser.h"
#include "include/logger.h"
#include <string.h>
#include <stdio.h>

// Initializes the parser with the given lexer and sets up the initial tokens.
parser_T *init_parser(lexer_T *lexer)
{
    // Check if lexxer is null
    if (lexer == NULL)
    {
        fprintf(stderr, "Lexer is null\n");
        exit(1);
    }

    // Check if lexer->i is greater than the length of the contents
    if (lexer->i > strlen(lexer->contents))
    {
        fprintf(stderr, "Lexer index is greater than the length of the contents\n");
        exit(1);
    }

    parser_T *parser = calloc(1, sizeof(struct PARSE_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->peek_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;
    return parser;
}

// Consumes the current token if it matches the expected type, otherwise throws an error.
void parser_eat(parser_T *parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = parser->peek_token;
        parser->peek_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        fprintf(stderr, "Unexpected token '%s', expected '%s'\n",
                token_type_to_string(parser->current_token->type),
                token_type_to_string(token_type));
        exit(1);
    }
}

// Parses the entire input and returns the root of the AST.
AST_T *parser_parse(parser_T *parser)
{
    AST_T *root = parser_parse_statements(parser);
    return root;
}

// Parses a series of statements and returns them as a compound AST node.
AST_T *parser_parse_statements(parser_T *parser)
{
    AST_T *compound = init_ast(AST_COMPOUND);
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT *));

    LOG_PRINT("Parsing statement: %s\n", parser->current_token->value);
    AST_T *ast_statement = parser_parse_statement(parser);

    compound->compound_value[0] = ast_statement;
    compound->compound_size++;

    LOG_PRINT("Parsing value: %s\n", parser->current_token->value);

    if (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);
    }

    while (parser->current_token->type != TOKEN_EOF &&
           parser->lexer->i < strlen(parser->lexer->contents) - 1 &&
           parser->current_token->type != TOKEN_RBRACE)
    {

        LOG_PRINT("Parsing statement: %s\n", parser->current_token->value);
        AST_T *ast_statement = parser_parse_statement(parser);

        compound->compound_size++;
        compound->compound_value = realloc(
            compound->compound_value,
            (compound->compound_size + 1) * sizeof(struct AST_STRUCT *));
        compound->compound_value[compound->compound_size - 1] = ast_statement;

        if (parser->current_token->type == TOKEN_SEMI)
        {
            parser_eat(parser, TOKEN_SEMI);
        }
    }

    return compound;
}

// Parses a single statement based on the current token type.
AST_T *parser_parse_statement(parser_T *parser)
{
    switch (parser->current_token->type)
    {
    case TOKEN_VARIABLE_DEFINITION:
        LOG_PRINT("Parsing variable definition\n");
        return parser_parse_variable_definition(parser);
    case TOKEN_ID:
        LOG_PRINT("Parsing variable\n");
        return parser_parse_id(parser);
    case TOKEN_STRING:
        LOG_PRINT("Parsing string\n");
        return parser_parse_string(parser);
    case TOKEN_INT:
        LOG_PRINT("Parsing int\n");
        return parser_parse_int(parser);
    case TOKEN_FUNCTION_DEFINITION:
        LOG_PRINT("Parsing function definition\n");
        return parser_parse_function_definition(parser);
    case TOKEN_RETURN:
        LOG_PRINT("Parsing return\n");
        return parser_parse_return(parser);
    case TOKEN_IF:
        LOG_PRINT("Parsing ifelse branch\n");
        return parser_parse_ifelse_statement(parser);
    case TOKEN_EOF:
        return NULL;
    default:
        fprintf(stderr, "Unexpected token '%s'\n",
                token_type_to_string(parser->current_token->type));
        exit(1);
    }
}

// Parses a variable definition statement.
AST_T *parser_parse_variable_definition(parser_T *parser)
{
    parser_eat(parser, TOKEN_VARIABLE_DEFINITION);

    char *variable_name = parser->current_token->value;
    AST_T *ast_variable_definition_variable_count = init_ast(AST_VARIABLE_COUNT);
    ast_variable_definition_variable_count->variable_count = 1;

    if (parser->current_token->type == TOKEN_INT)
    {
        ast_variable_definition_variable_count = parser_parse_variable_count(parser);
        variable_name = parser->current_token->value;
        LOG_PRINT("Variable count: %d\n", ast_variable_definition_variable_count->variable_count);
    }

    parser_eat(parser, TOKEN_ID);

    LOG_PRINT("Variable name: %s\n", variable_name);

    parser_eat(parser, TOKEN_EQUALS);

    AST_T *ast_variable_definition_value = parser_parse_expression(parser);

    ast_print(ast_variable_definition_value, 0);
    AST_T *ast_variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    ast_variable_definition->variable_definition_variable_name = variable_name;
    ast_variable_definition->variable_definition_value = ast_variable_definition_value;
    ast_variable_definition->variable_definition_variable_count = ast_variable_definition_variable_count;

    return ast_variable_definition;
}

// Parses a variable or function call.
AST_T *parser_parse_variable(parser_T *parser)
{
    char *token_name = parser->current_token->value;
    LOG_PRINT("Parsing variable: %s\n", token_name);

    // Check if the variable is an int
    if (atoi(token_name) != 0 || strcmp(token_name, "0") == 0)
    {
        AST_T *ast_int = init_ast(AST_INT);
        ast_int->int_value = atoi(token_name);
        parser_eat(parser, TOKEN_INT);
        return ast_int;
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

    AST_T *ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_name;

    parser_eat(parser, parser->current_token->type);

    return ast_variable;
}

AST_T *parser_parse_variable_count(parser_T *parser)
{
    LOG_PRINT("Parsing variable count\n");

    AST_T *ast_variable_count = init_ast(AST_VARIABLE_COUNT);
    int count = atoi(parser->current_token->value);

    if (count <= 0)
    {
        fprintf(stderr, "Variable count must be greater than 0\n");
        exit(1);
    }

    ast_variable_count->variable_count = count;

    parser_eat(parser, TOKEN_INT);

    return ast_variable_count;
}

AST_T *parser_parse_function_argument(parser_T *parser)
{
    LOG_PRINT("Parsing function argument\n");
    char *variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    AST_T *ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = variable_name;

    return ast_variable;
}

AST_T *parser_parse_function_definition(parser_T *parser)
{
    parser_eat(parser, TOKEN_FUNCTION_DEFINITION);

    char *function_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    parser_eat(parser, TOKEN_LPAREN);

    AST_T *ast_function_definition = init_ast(AST_FUNCTION_DEFINITION);
    ast_function_definition->function_definition_name = function_name;
    ast_function_definition->function_definition_arguments = calloc(1, sizeof(struct AST_STRUCT *));

    while (parser->current_token->type != TOKEN_RPAREN)
    {
        AST_T *ast_variable = parser_parse_function_argument(parser);

        int last_idx = ++ast_function_definition->function_definition_arguments_size;
        ast_function_definition->function_definition_arguments = realloc(
            ast_function_definition->function_definition_arguments,
            (last_idx + 1) * sizeof(struct AST_STRUCT *));

        ast_function_definition->function_definition_arguments[last_idx - 1] = ast_variable;

        LOG_PRINT("Argument: %s\n", parser->prev_token->value);

        if (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);
        }
    }

    parser_eat(parser, TOKEN_RPAREN);
    parser_eat(parser, TOKEN_LBRACE);
    LOG_PRINT("Parsing function body\n");
    ast_function_definition->function_definition_body = parser_parse_statements(parser);
    LOG_PRINT("Parsed function body\n");
    parser_eat(parser, TOKEN_RBRACE);

    return ast_function_definition;
}

AST_T *parser_parse_return(parser_T *parser)
{
    parser_eat(parser, TOKEN_RETURN);

    AST_T *ast_return = init_ast(AST_RETURN);
    ast_return->return_value = parser_parse_expression(parser);

    return ast_return;
}

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
        return 0;
    }
}

// Parses an expression based on the current token type with precedence handling.
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
            type = AST_PLUS;
            break;
        case TOKEN_MINUS:
            type = AST_MINUS;
            break;
        case TOKEN_GT:
            type = AST_GT;
            break;
        case TOKEN_LT:
            type = AST_LT;
            break;
        case TOKEN_GTE:
            type = AST_GTE;
            break;
        case TOKEN_LTE:
            type = AST_LTE;
            break;
        case TOKEN_AND:
            type = AST_AND;
            break;
        case TOKEN_OR:
            type = AST_OR;
            break;
        case TOKEN_EQUAL:
            type = AST_EQUAL;
            break;
        case TOKEN_MUL:
            type = AST_MUL;
            break;
        case TOKEN_DIV:
            type = AST_DIV;
            break;
        }
        AST_T *new_node = init_ast(type);
        new_node->left = left;
        new_node->right = right;
        left = new_node;
        LOG_PRINT("Added expression: %s\n", token_type_to_string(token_type));
        LOG_PRINT("\tLeft: %s\n", ast_type_to_string(left->left->type));
        LOG_PRINT("\tRight: %s\n", ast_type_to_string(left->right->type));
    }
    return left;
}

// Parses an expression based on the current token type.
AST_T *parser_parse_expression(parser_T *parser)
{
    return parser_parse_expression_with_precedence(parser, 0);
}

// Parses a factor based on the current token type with precedence handling.
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
    else if (parser->current_token->type == TOKEN_LPAREN)
    {
        parser_eat(parser, TOKEN_LPAREN);
        node = init_ast(AST_NESTED_EXPRESSION);
        node->nested_expression = parser_parse_expression_with_precedence(parser, 0);
        parser_eat(parser, TOKEN_RPAREN);
    }
    else if (parser->current_token->type == TOKEN_NOT)
    {
        parser_eat(parser, TOKEN_NOT);
        node = init_ast(AST_NOT);
        node->not_expression = parser_parse_factor_with_precedence(parser, get_precedence(TOKEN_NOT));
    }
    else
    {
        fprintf(stderr, "Unexpected token: %s\n", token_type_to_string(parser->current_token->type));
        exit(1);
    }
    return node;
}

// Parses a factor based on the current token type.
AST_T *parser_parse_factor(parser_T *parser)
{
    return parser_parse_factor_with_precedence(parser, 0);
}

AST_T *parser_parse_ifelse_statement(parser_T *parser)
{
    AST_T *ast_if = init_ast(AST_IF);
    parser_eat(parser, TOKEN_IF);
    parser_eat(parser, TOKEN_LPAREN);
    ast_if->if_condition = parser_parse_expression(parser);
    parser_eat(parser, TOKEN_RPAREN);

    parser_eat(parser, TOKEN_LBRACE);
    ast_if->if_body = parser_parse_statements(parser);
    parser_eat(parser, TOKEN_RBRACE);

    AST_T *ast_ifelse = init_ast(AST_IF_BRANCH);
    ast_ifelse->if_else_compound_value = calloc(1, sizeof(struct AST_STRUCT *));
    ast_ifelse->if_else_compound_size = 1;
    ast_ifelse->if_else_compound_value[0] = ast_if;

    while (parser->current_token->type == TOKEN_ELSEIF)
    {
        LOG_PRINT("Parsing elseif\n");
        AST_T *ast_elseif = init_ast(AST_ELSEIF);
        parser_eat(parser, TOKEN_ELSEIF);
        parser_eat(parser, TOKEN_LPAREN);
        ast_elseif->elseif_condition = parser_parse_expression(parser);
        parser_eat(parser, TOKEN_RPAREN);

        parser_eat(parser, TOKEN_LBRACE);
        ast_elseif->elseif_body = parser_parse_statements(parser);
        parser_eat(parser, TOKEN_RBRACE);

        ast_ifelse->if_else_compound_size++;
        ast_ifelse->if_else_compound_value = realloc(
            ast_ifelse->if_else_compound_value,
            ast_ifelse->if_else_compound_size * sizeof(struct AST_STRUCT *));

        ast_ifelse->if_else_compound_value[ast_ifelse->if_else_compound_size - 1] = ast_elseif;
    }

    if (parser->current_token->type == TOKEN_ELSE)
    {
        LOG_PRINT("Parsing else\n");
        AST_T *ast_else = init_ast(AST_ELSE);
        parser_eat(parser, TOKEN_ELSE);
        parser_eat(parser, TOKEN_LBRACE);
        ast_else->else_body = parser_parse_statements(parser);
        parser_eat(parser, TOKEN_RBRACE);
        ast_ifelse->if_else_compound_size++;
        ast_ifelse->if_else_compound_value = realloc(
            ast_ifelse->if_else_compound_value,
            ast_ifelse->if_else_compound_size * sizeof(struct AST_STRUCT *));

        ast_ifelse->if_else_compound_value[ast_ifelse->if_else_compound_size - 1] = ast_else;
    }

    return ast_ifelse;
}

AST_T *parser_parse_function_call(parser_T *parser)
{
    char *function_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_LPAREN);

    AST_T *ast_function_call = init_ast(AST_FUNCTION_CALL);
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

    return ast_function_call;
}

AST_T *parser_parse_id(parser_T *parser)
{
    LOG_PRINT("Parsing id: %s\n", parser->current_token->value);
    char *token_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_LPAREN)
    {
        return parser_parse_function_call(parser);
    }

    AST_T *ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_name;

    return ast_variable;
}

AST_T *parser_parse_int(parser_T *parser)
{
    int value = atoi(parser->current_token->value);
    parser_eat(parser, TOKEN_INT);

    AST_T *ast_int = init_ast(AST_INT);
    ast_int->int_value = value;

    return ast_int;
}

AST_T *parser_parse_string(parser_T *parser)
{
    char *value = parser->current_token->value;
    parser_eat(parser, TOKEN_STRING);

    AST_T *ast_string = init_ast(AST_STRING);
    ast_string->string_value = value;

    return ast_string;
}
