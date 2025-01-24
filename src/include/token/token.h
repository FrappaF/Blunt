#ifndef TOKEN_H
#define TOKEN_H

#include "token_definitions.h"
/**
 * Structure representing a token.
 * @var value The value of the token.
 * @var type The type of the token.
 */
typedef struct TOKEN_STRUCT
{
    char *value;
    int type;
} token_T;

/**
 * Initializes a token with the given type and value.
 * @param type The type of the token.
 * @param value The value of the token.
 * @return A pointer to the initialized token.
 */
token_T *init_token(int type, char *value);

/**
 * Converts a token type to its string representation.
 * @param type The type of the token.
 * @return The string representation of the token type.
 */
const char *token_type_to_string(int type);

/**
 * Structure representing a mapping between a character and a token type.
 * @var character The character to map.
 * @var token_type The token type to map to.
 */
typedef struct
{
    char character;
    int token_type;
} lexer_token_map_T;

extern lexer_token_map_T token_map[];

/**
 * Structure representing a mapping between an identifier name and a token type.
 * @var id_name The identifier name to map.
 * @var token_type The token type to map to.
 */
typedef struct
{
    char *id_name;
    int token_type;
} lexer_id_map_T;

extern lexer_id_map_T id_map[];

#endif // TOKEN_H