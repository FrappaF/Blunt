# Lexer

The lexer is a crucial component of the Blunt compiler. It is responsible for converting the input source code into a sequence of tokens, which are then used by the parser to generate the abstract syntax tree (AST).

## Overview

The lexer processes the input source code character by character, identifying meaningful sequences called tokens. Each token represents a basic element of the language, such as keywords, identifiers, operators, and literals.

## Key Functions

- **Initialization**: The lexer is initialized with the input source code.
- **Advancement**: The lexer advances through the input one character at a time.
- **Skipping Whitespace and Comments**: The lexer can skip over whitespace and comments to focus on meaningful tokens.
- **Token Collection**: The lexer collects different types of tokens, such as identifiers and strings.
- **Token Retrieval**: The lexer retrieves the next token from the input.

## Usage

To use the lexer, initialize it with the input source code and repeatedly call `lexer_get_next_token` to retrieve tokens until the end of the input is reached.

## Example

```c
#include "lexer/lexer.h"

int main() {
    char *source_code = "int main() { return 0; }";
    lexer_T *lexer = init_lexer(source_code);

    token_T *token = NULL;
    while ((token = lexer_get_next_token(lexer)) != NULL) {
        // Process the token
    }

    return 0;
}
```

This example demonstrates how to initialize the lexer with a simple source code string and retrieve tokens in a loop.

## Structure

The lexer is defined by the `lexer_T` structure, which includes the current character, the current position in the input, and the input contents.

```c
typedef struct LEXER_STRUCT {
    char c;
    unsigned int i;
    char *contents;
} lexer_T;
```

## Functions

The lexer provides several functions for initialization, advancement, skipping, and token collection. Refer to the `lexer.h` header file for detailed documentation of each function.

```c
lexer_T *init_lexer(char *contents);
void lexer_advance(lexer_T *lexer);
void lexer_skip_whitespace(lexer_T *lexer);
void lexer_skip_comment(lexer_T *lexer);
token_T *lexer_get_next_token(lexer_T *lexer);
token_T *lexer_advance_with_token(lexer_T *lexer, token_T *token);
token_T *lexer_collect_id(lexer_T *lexer);
token_T *lexer_collect_string(lexer_T *lexer);
char *lexer_get_current_char_as_string(lexer_T *lexer);
```

## Conclusion

The lexer is an essential part of the Blunt compiler, enabling the conversion of source code into tokens for further processing by the parser. By understanding and utilizing the lexer, you can effectively tokenize and analyze Blunt source code.
