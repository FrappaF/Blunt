# Abstract Syntax Tree (AST) Overview

This document provides an overview of the Abstract Syntax Tree (AST) structures used in the Blunt project. The AST is a crucial part of the compiler, representing the syntactic structure of the source code.

## AST Types

The AST nodes are categorized into various types, each representing a different syntactic construct. The types are defined in the `AST.h` file.

### AST Node Types

- `AST_VARIABLE_DEFINITION`
- `AST_VARIABLE`
- `AST_VARIABLE_COUNT`
- `AST_VARIABLE_ASSIGNMENT`
- `AST_FUNCTION_DEFINITION`
- `AST_RUNTIME_FUNCTION_DEFINITION`
- `AST_FUNCTION_CALL`
- `AST_RETURN`
- `AST_STRING`
- `AST_INT`
- `AST_ARRAY`
- `AST_COMPOUND`
- `AST_ADD_OP`
- `AST_SUB_OP`
- `AST_MUL_OP`
- `AST_DIV_OP`
- `AST_GT_OP`
- `AST_LT_OP`
- `AST_GTE_OP`
- `AST_LTE_OP`
- `AST_AND_OP`
- `AST_OR_OP`
- `AST_EQUAL_OP`
- `AST_NOT`
- `AST_NESTED_EXPRESSION`
- `AST_IF`
- `AST_ELSE`
- `AST_ELSEIF`
- `AST_IF_ELSE_BRANCH`
- `AST_DOT_EXPRESSION`
- `AST_DOT_DOT_EXPRESSION`
- `AST_FOR_LOOP`
- `AST_SAVE`
- `AST_DOT_DOT`
- `AST_NOOP`

## AST Structures

### Variable Nodes

Defined in `AST_variable.h`:

- `AST_VARIABLE_COUNT_T`
- `AST_VARIABLE_DEFINITION_T`
- `AST_VARIABLE_T`
- `AST_VARIABLE_ASSIGNMENT_T`

### Operation Nodes

Defined in `AST_operations.h`:

- `AST_ADD_OP_T`
- `AST_SUB_OP_T`
- `AST_MUL_OP_T`
- `AST_DIV_OP_T`
- `AST_AND_OP_T`
- `AST_OR_OP_T`
- `AST_GT_OP_T`
- `AST_LT_OP_T`
- `AST_GTE_OP_T`
- `AST_LTE_OP_T`
- `AST_EQUAL_OP_T`
- `AST_NOT_T`

### Miscellaneous Nodes

Defined in `AST_misc.h`:

- `AST_STRING_T`
- `AST_INT_T`
- `AST_ARRAY_T`
- `AST_COMPOUND_T`
- `AST_NESTED_EXPRESSION_T`
- `AST_RETURN_T`
- `AST_DOT_EXPRESSION_T`
- `AST_DOT_DOT_T`
- `AST_DOT_DOT_EXPRESSION_T`
- `AST_SAVE_T`

### Function Nodes

Defined in `AST_function.h`:

- `AST_FUNCTION_CALL_T`
- `AST_FUNCTION_DEFINITION_T`
- `AST_RUNTIME_FUNCTION_DEFINITION_T`

### Control Flow Nodes

Defined in `AST_control_flow.h`:

- `AST_IF_ELSE_BRANCH_T`
- `AST_IF_T`
- `AST_ELSE_T`
- `AST_ELSEIF_T`
- `AST_FOR_LOOP_T`

## Usage

Each AST node type has a corresponding structure and functions to initialize and manipulate the nodes. The parser generates these nodes as it processes the source code, building a tree that represents the entire program.

For more details on each structure and its fields, refer to the respective header files.
