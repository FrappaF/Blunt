# Token and Token Definitions

The `token` module is responsible for representing and managing tokens in the language. Tokens are the smallest units of meaning in the source code, such as keywords, operators, and identifiers. The `token_definitions` module defines the various token types used in the language.

## Token Structure

The `token_T` structure represents a token with the following fields:

- `value`: The value of the token.
- `type`: The type of the token.

## Token Functions

- `init_token(int type, char *value)`: Initializes a token with the given type and value.
- `token_type_to_string(int type)`: Converts a token type to its string representation.

## Token Definitions

The `token_definitions.h` file defines the various token types used in the language. These definitions include tokens for operators, keywords, literals, and other symbols.

## Usage

The token module is used during the lexical analysis phase to convert the source code into a stream of tokens. These tokens are then used by the parser to construct the abstract syntax tree (AST).

```c
// Example usage
token_T *token = init_token(TOKEN_ID, "variable_name");
printf("Token type: %s\n", token_type_to_string(token->type));
printf("Token value: %s\n", token->value);
```

This module is essential for the lexical analysis phase and provides the necessary structures and functions to manage tokens in the language.
