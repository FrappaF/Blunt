#include "../include/parser/parser_statements.h"
#include "../include/io/logger.h"
#include <string.h>
#include <stdio.h>

// Parses a series of statements and returns them as a compound AST node.
AST_T *parser_parse_statements(parser_T *parser)
{
    AST_COMPOUND_T *compound = (AST_COMPOUND_T *)init_ast(AST_COMPOUND);
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

    return (AST_T *)compound;
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

    case TOKEN_FOR:
        LOG_PRINT("Parsing for loop\n");
        return parser_parse_for_loop(parser);

    case TOKEN_SAVE:
        LOG_PRINT("Parsing save\n");
        return parser_parse_save_statement(parser);

    case TOKEN_DOT:
        LOG_PRINT("Parsing dot expression\n");
        return parser_parse_dot_expression(parser);

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
    AST_VARIABLE_COUNT_T *ast_variable_definition_variable_count = (AST_VARIABLE_COUNT_T *)init_ast(AST_VARIABLE_COUNT);
    ast_variable_definition_variable_count->variable_count_value = 1;

    if (parser->current_token->type == TOKEN_INT)
    {
        ast_variable_definition_variable_count = (AST_VARIABLE_COUNT_T *)parser_parse_variable_count(parser);
        variable_name = parser->current_token->value;
        LOG_PRINT("Variable count: %d\n", ast_variable_definition_variable_count->variable_count_value);
    }

    parser_eat(parser, TOKEN_ID);

    LOG_PRINT("Variable name: %s\n", variable_name);

    parser_eat(parser, TOKEN_EQUALS);

    AST_T *ast_variable_definition_value = parser_parse_expression(parser);

    ast_print(ast_variable_definition_value, 0);
    AST_VARIABLE_DEFINITION_T *ast_variable_definition = (AST_VARIABLE_DEFINITION_T *)init_ast(AST_VARIABLE_DEFINITION);
    ast_variable_definition->variable_definition_variable_name = variable_name;
    ast_variable_definition->variable_definition_value = ast_variable_definition_value;
    ast_variable_definition->variable_definition_variable_count = ast_variable_definition_variable_count;

    return (AST_T *)ast_variable_definition;
}

// Parses a function definition.
AST_T *parser_parse_function_definition(parser_T *parser)
{
    parser_eat(parser, TOKEN_FUNCTION_DEFINITION);

    char *function_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    parser_eat(parser, TOKEN_LPAREN);

    AST_FUNCTION_DEFINITION_T *ast_function_definition = (AST_FUNCTION_DEFINITION_T *)init_ast(AST_FUNCTION_DEFINITION);
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

    return (AST_T *)ast_function_definition;
}

// Parses a return statement.
AST_T *parser_parse_return(parser_T *parser)
{
    parser_eat(parser, TOKEN_RETURN);

    AST_RETURN_T *ast_return = (AST_RETURN_T *)init_ast(AST_RETURN);
    ast_return->return_value = parser_parse_expression(parser);

    return (AST_T *)ast_return;
}

// Parses a compound ifelse statement.
AST_T *parser_parse_ifelse_statement(parser_T *parser)
{
    AST_IF_T *ast_if = (AST_IF_T *)init_ast(AST_IF);
    parser_eat(parser, TOKEN_IF);
    parser_eat(parser, TOKEN_LPAREN);
    ast_if->if_condition = parser_parse_expression(parser);
    parser_eat(parser, TOKEN_RPAREN);

    parser_eat(parser, TOKEN_LBRACE);
    ast_if->if_body = parser_parse_statements(parser);
    parser_eat(parser, TOKEN_RBRACE);

    AST_IF_ELSE_BRANCH_T *ast_ifelse = (AST_IF_ELSE_BRANCH_T *)init_ast(AST_IF_ELSE_BRANCH);
    ast_ifelse->if_else_compound_value = calloc(1, sizeof(struct AST_STRUCT *));
    ast_ifelse->if_else_compound_size = 1;
    ast_ifelse->if_else_compound_value[0] = (AST_T *)ast_if;

    while (parser->current_token->type == TOKEN_ELSEIF)
    {
        LOG_PRINT("Parsing elseif\n");
        AST_ELSEIF_T *ast_elseif = (AST_ELSEIF_T *)init_ast(AST_ELSEIF);
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

        ast_ifelse->if_else_compound_value[ast_ifelse->if_else_compound_size - 1] = (AST_T *)ast_elseif;
    }

    if (parser->current_token->type == TOKEN_ELSE)
    {
        LOG_PRINT("Parsing else\n");
        AST_ELSE_T *ast_else = (AST_ELSE_T *)init_ast(AST_ELSE);
        parser_eat(parser, TOKEN_ELSE);
        parser_eat(parser, TOKEN_LBRACE);
        ast_else->else_body = parser_parse_statements(parser);
        parser_eat(parser, TOKEN_RBRACE);
        ast_ifelse->if_else_compound_size++;
        ast_ifelse->if_else_compound_value = realloc(
            ast_ifelse->if_else_compound_value,
            ast_ifelse->if_else_compound_size * sizeof(struct AST_STRUCT *));

        ast_ifelse->if_else_compound_value[ast_ifelse->if_else_compound_size - 1] = (AST_T *)ast_else;
    }

    return (AST_T *)ast_ifelse;
}

// Parses a for loop.
AST_T *parser_parse_for_loop(parser_T *parser)
{
    LOG_PRINT("Parsing for loop\n");
    parser_eat(parser, TOKEN_FOR);

    AST_FOR_LOOP_T *ast_for = (AST_FOR_LOOP_T *)init_ast(AST_FOR_LOOP);

    LOG_PRINT("Parsing for loop variable\n");
    ast_for->for_loop_variable = parser_parse_id(parser);

    LOG_PRINT("Parsing for loop iterator\n");
    // Check if the current token is a for iterator otherwise create a new variable
    if (parser->current_token->type != TOKEN_FOR_ITERATOR)
    {
        LOG_PRINT("Creating for loop iterator\n");
        char *iterator_name = "i";
        AST_VARIABLE_T *ast_variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
        ast_variable->variable_name = iterator_name;
        ast_for->for_loop_increment = (AST_T *)ast_variable;
    }
    else
    {
        parser_eat(parser, TOKEN_FOR_ITERATOR);
        char *token_name = parser->current_token->value;
        AST_VARIABLE_T *ast_variable = (AST_VARIABLE_T *)init_ast(AST_VARIABLE);
        ast_variable->variable_name = token_name;
        ast_for->for_loop_increment = (AST_T *)ast_variable;

        LOG_PRINT("Parsing for loop condition\n");
        ast_for->for_loop_condition = parser_parse_expression(parser);
    }

    LOG_PRINT("Parsing for loop body\n");
    parser_eat(parser, TOKEN_LBRACE);

    ast_for->for_loop_body = parser_parse_statements(parser);

    parser_eat(parser, TOKEN_RBRACE);

    return (AST_T *)ast_for;
}

// Parses a save statement.
AST_T *parser_parse_save_statement(parser_T *parser)
{
    LOG_PRINT("Parsing save\n");
    parser_eat(parser, TOKEN_SAVE);

    AST_SAVE_T *ast_save = (AST_SAVE_T *)init_ast(AST_SAVE);
    ast_save->save_value = (AST_VARIABLE_T *)parser_parse_id(parser);

    return (AST_T *)ast_save;
}
