#ifndef LEXER_H
#define LEXER_H

#include "token.h"

/**
 * Structure representing the lexer.
 * @var c The current character being processed.
 * @var i The current position in the input.
 * @var contents The input contents to be tokenized.
 */
typedef struct LEXER_STRUCT
{
    char c;
    unsigned int i;
    char *contents;
} lexer_T;

/**
 * Initializes the lexer with the given input contents.
 * @param contents The input contents to be tokenized.
 * @return A pointer to the initialized lexer.
 */
lexer_T *init_lexer(char *contents);

/**
 * Advances the lexer by one character.
 * @param lexer The lexer instance.
 */
void lexer_advance(lexer_T *lexer);

/**
 * Skips all whitespace and newline characters.
 * @param lexer The lexer instance.
 */
void lexer_skip_whitespace(lexer_T *lexer);

/**
 * Skips comments until a newline character is found.
 * @param lexer The lexer instance.
 */
void lexer_skip_comment(lexer_T *lexer);

/**
 * Gets the next token from the lexer.
 * @param lexer The lexer instance.
 * @return The next token.
 */
token_T *lexer_get_next_token(lexer_T *lexer);

/**
 * Advances the lexer and returns the given token.
 * @param lexer The lexer instance.
 * @param token The token to return.
 * @return The given token.
 */
token_T *lexer_advance_with_token(lexer_T *lexer, token_T *token);

/**
 * Collects an identifier token.
 * @param lexer The lexer instance.
 * @return The collected identifier token.
 */
token_T *lexer_collect_id(lexer_T *lexer);

/**
 * Collects a string token.
 * @param lexer The lexer instance.
 * @return The collected string token.
 */
token_T *lexer_collect_string(lexer_T *lexer);

/**
 * Gets the current character as a string.
 * @param lexer The lexer instance.
 * @return The current character as a string.
 */
char *lexer_get_current_char_as_string(lexer_T *lexer);

#endif // LEXER_H