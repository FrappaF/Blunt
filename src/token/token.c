#include "../include/token/token.h"
#include "../include/io/logger.h"
#include <stdlib.h>

// Initialize a token with the given type and value
token_T *init_token(int type, char *value)
{
    token_T *token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;
    token->value = value;
    return token;
}

// Token map for character to token type conversion

lexer_token_map_T token_map[] = {
    {'=', TOKEN_EQUALS},
    {';', TOKEN_SEMI},
    {'(', TOKEN_LPAREN},
    {')', TOKEN_RPAREN},
    {'{', TOKEN_LBRACE},
    {'}', TOKEN_RBRACE},
    {',', TOKEN_COMMA},
    {'+', TOKEN_PLUS},
    {'-', TOKEN_MINUS},
    {'*', TOKEN_MUL},
    {'/', TOKEN_DIV},
    {'<', TOKEN_LT},
    {'>', TOKEN_GT},
    {'[', TOKEN_LSQUARE},
    {']', TOKEN_RSQUARE},
    {'.', TOKEN_DOT},
    {'\0', 0} // End of map marker
};

lexer_id_map_T id_map[] = {
    {ID_DEF_VAR, TOKEN_VARIABLE_DEFINITION},
    {ID_RETURN, TOKEN_RETURN},
    {ID_FUNCTION_DEFINITION, TOKEN_FUNCTION_DEFINITION},
    {ID_ASSIGNMENT, TOKEN_EQUALS},
    {ID_IF, TOKEN_IF},
    {ID_ELSEIF, TOKEN_ELSEIF},
    {ID_ELSE, TOKEN_ELSE},
    {ID_AND, TOKEN_AND},
    {ID_OR, TOKEN_OR},
    {ID_NOT, TOKEN_NOT},
    {ID_FOR, TOKEN_FOR},
    {ID_FOR_ITERATOR, TOKEN_FOR_ITERATOR},
    {ID_SAVE, TOKEN_SAVE},
    {"\0", 0} // End of map marker
};

const char *token_type_to_string(int type)
{
    switch (type)
    {
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_SEMI:
        return "TOKEN_SEMI";
    case TOKEN_LPAREN:
        return "TOKEN_LPAREN";
    case TOKEN_RPAREN:
        return "TOKEN_RPAREN";
    case TOKEN_LBRACE:
        return "TOKEN_LBRACE";
    case TOKEN_RBRACE:
        return "TOKEN_RBRACE";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_PLUS:
        return "TOKEN_PLUS";
    case TOKEN_MINUS:
        return "TOKEN_MINUS";
    case TOKEN_MUL:
        return "TOKEN_MUL";
    case TOKEN_DIV:
        return "TOKEN_DIV";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_ID:
        return "TOKEN_ID";
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_VARIABLE_DEFINITION:
        return "TOKEN_VARIABLE_DEFINITION";
    case TOKEN_RETURN:
        return "TOKEN_RETURN";
    case TOKEN_FUNCTION_DEFINITION:
        return "TOKEN_FUNCTION_DEFINITION";
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_GT:
        return "TOKEN_GT";
    case TOKEN_LT:
        return "TOKEN_LT";
    case TOKEN_GTE:
        return "TOKEN_GTE";
    case TOKEN_LTE:
        return "TOKEN_LTE";
    case TOKEN_IF:
        return "TOKEN_IF";
    case TOKEN_ELSE:
        return "TOKEN_ELSE";
    case TOKEN_ELSEIF:
        return "TOKEN_ELSEIF";
    case TOKEN_AND:
        return "TOKEN_AND";
    case TOKEN_OR:
        return "TOKEN_OR";
    case TOKEN_NOT:
        return "TOKEN_NOT";
    case TOKEN_EQUAL:
        return "TOKEN_EQUAL";
    case TOKEN_LSQUARE:
        return "TOKEN_LSQUARE";
    case TOKEN_RSQUARE:
        return "TOKEN_RSQUARE";
    case TOKEN_DOT:
        return "TOKEN_DOT";
    case TOKEN_FOR:
        return "TOKEN_FOR";
    case TOKEN_FOR_ITERATOR:
        return "TOKEN_FOR_ITER";
    case TOKEN_SAVE:
        return "TOKEN_SAVE";
    default:
        LOG_PRINT("Unknown token type: %d\n", type);
        return "TOKEN_UNKNOWN";
    }
}