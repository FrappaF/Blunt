# Blunt Parser

## Overview

The parser in Blunt is responsible for converting the tokenized input from the lexer into an Abstract Syntax Tree (AST). The AST represents the hierarchical structure of the source code, which the visitor can then interpret.

## Parser Structure

The parser is structured to handle different types of statements and expressions. It uses a lexer to tokenize the input and then builds the AST by parsing these tokens.

## Key Components

### Parser Initialization

The parser is initialized with a lexer and starts parsing from the first token.

### Statement and Expression Parsing

The parser provides functions to parse different types of statements and expressions, such as variable definitions, function calls, and control flow statements. Each parse function processes the tokens and builds the corresponding AST nodes.

## File Structure

- `parser.h`: Main parser structure and function declarations.
- `parser_statements.h`: Functions for parsing statement-related nodes.
- `parser_expressions.h`: Functions for parsing expression-related nodes.

## Usage

To use the parser, initialize it with a lexer and call the `parser_parse` function to get the root node of the AST.

```c
parser_T *parser = init_parser(lexer);
AST_T *root_node = parser_parse(parser);
```

## Conclusion

The parser in Blunt is a crucial component for interpreting the code and producing the result. It converts the tokenized input into an AST, which the visitor can then traverse to execute the code. Together, they provide the core functionality for the language.
