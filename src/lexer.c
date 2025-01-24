#include "include/lexer/lexer.h"
#include "include/token/token.h"
#include "include/io/logger.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initialize the lexer with the given contents
lexer_T *init_lexer(char *contents)
{
    lexer_T *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];
    return lexer;
}

// Advance the lexer by one character
void lexer_advance(lexer_T *lexer)
{
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

// Skip all whitespace and newline characters
void lexer_skip_whitespace(lexer_T *lexer)
{
    while (lexer->c == ' ' || lexer->c == 10)
    {
        // printf("Skipping whitespace %d\n", lexer->c);
        lexer_advance(lexer);
    }
}

// Skip comments until a newline character is found
void lexer_skip_comment(lexer_T *lexer)
{
    lexer_skip_whitespace(lexer);

    while (lexer->c == ID_COMMENT)
    {
        LOG_PRINT("Skipping comment line\n");
        while (lexer->c != 10)
        {
            lexer_advance(lexer);
        }

        lexer_skip_whitespace(lexer);
    }
}

// Get the next token from the lexer
token_T *lexer_get_next_token(lexer_T *lexer)
{
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents) - 1)
    {
        lexer_skip_comment(lexer);
        lexer_skip_whitespace(lexer);

        // Check the end of file
        if (lexer->c == '\0')
        {
            return init_token(TOKEN_EOF, "\0");
        }

        if (isalnum(lexer->c))
        {
            token_T *id_token = lexer_collect_id(lexer);
            // Check if the identifier is an int
            if (atoi(id_token->value) != 0 || strcmp(id_token->value, "0") == 0)
            {
                id_token->type = TOKEN_INT;
                return id_token;
            }

            for (int i = 0; strcmp(id_map[i].id_name, "\0") != 0; i++)
            {
                if (strcmp(id_token->value, id_map[i].id_name) == 0)
                {
                    id_token->type = id_map[i].token_type;
                    return id_token;
                }
            }

            return id_token;
        }
        else if (lexer->c == '"' || lexer->c == '\'')
        {
            return lexer_collect_string(lexer);
        }
        for (int j = 0; token_map[j].character != '\0'; j++)
        {
            if (lexer->c == token_map[j].character)
            {
                if (token_map[j].token_type == TOKEN_GT)
                {
                    lexer_advance(lexer);
                    if (lexer->c == '=')
                    {
                        return lexer_advance_with_token(lexer, init_token(TOKEN_GTE, ">="));
                    }
                    return init_token(token_map[j].token_type, ">");
                }
                else if (token_map[j].token_type == TOKEN_LT)
                {
                    lexer_advance(lexer);
                    if (lexer->c == '=')
                    {
                        return lexer_advance_with_token(lexer, init_token(TOKEN_LTE, "<="));
                    }
                    return init_token(token_map[j].token_type, "<");
                }
                else if (token_map[j].token_type == TOKEN_EQUALS)
                {
                    lexer_advance(lexer);
                    if (lexer->c == '=')
                    {
                        return lexer_advance_with_token(lexer, init_token(TOKEN_EQUAL, "=="));
                    }
                    return init_token(token_map[j].token_type, "=");
                }
                else
                {
                    return lexer_advance_with_token(lexer, init_token(token_map[j].token_type, lexer_get_current_char_as_string(lexer)));
                }
            }
        }

        LOG_PRINT("Unexpected character: %c (%d)\n", lexer->c, lexer->c);
        exit(1);
    }

    if (lexer->i == strlen(lexer->contents) - 1)
        return lexer_advance_with_token(lexer, init_token(TOKEN_EOF, "\0"));

    return NULL;
}

// Advance the lexer and return the given token
token_T *lexer_advance_with_token(lexer_T *lexer, token_T *token)
{
    lexer_advance(lexer);
    return token;
}

// Collect a string token
token_T *lexer_collect_string(lexer_T *lexer)
{
    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != '"')
    {
        if (lexer->c == '\\')
        {
            lexer_advance(lexer);
            switch (lexer->c)
            {
            case 'n':
                value = realloc(value, (strlen(value) + 2) * sizeof(char));
                strcat(value, "\n");
                break;
            case 't':
                value = realloc(value, (strlen(value) + 2) * sizeof(char));
                strcat(value, "\t");
                break;
            case '\\':
                value = realloc(value, (strlen(value) + 2) * sizeof(char));
                strcat(value, "\\");
                break;
            case '"':
                value = realloc(value, (strlen(value) + 2) * sizeof(char));
                strcat(value, "\"");
                break;
            default:
                value = realloc(value, (strlen(value) + 2) * sizeof(char));
                strncat(value, &lexer->c, 1);
                break;
            }
        }
        else
        {
            char *s = lexer_get_current_char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
            strcat(value, s);
        }
        lexer_advance(lexer);
    }

    lexer_advance(lexer);
    return init_token(TOKEN_STRING, value);
}

// Get the current character as a string
char *lexer_get_current_char_as_string(lexer_T *lexer)
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';
    return str;
}

// Collect an identifier token
token_T *lexer_collect_id(lexer_T *lexer)
{
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advance(lexer);
    }

    return init_token(TOKEN_ID, value);
}
