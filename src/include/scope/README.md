# Scope

The `scope` module is responsible for maintaining function and variable definitions during the visitor's traversal of the abstract syntax tree (AST). It provides structures and functions to manage these definitions and ensure they are accessible when needed.

## Structures

- `scope_T`: Represents a scope containing function and variable definitions.
- `scope_stack_T`: Represents a stack of scopes, allowing nested scopes.

## Functions

- `init_scope()`: Initializes a new scope.
- `init_scope_stack()`: Initializes a new scope stack.
- `free_scope_stack(scope_stack_T *stack)`: Frees the memory allocated for the scope stack.
- `print_scope(scope_T *scope)`: Prints the contents of a scope.
- `push_scope_to_stack(scope_stack_T *stack, scope_T *scope)`: Pushes a new scope onto the stack.
- `pop_scope_from_stack(scope_stack_T *stack)`: Pops the top scope from the stack.
- `scope_add_function_definition(scope_T *scope, AST_T *fdef)`: Adds a function definition to the scope.
- `scope_get_function_definition(scope_T *scope, const char *fname)`: Retrieves a function definition from the scope by name.
- `scope_add_variable_definition(scope_T *scope, AST_T *vdef)`: Adds a variable definition to the scope.
- `scope_get_variable_definition(scope_T *scope, const char *name)`: Retrieves a variable definition from the scope by name.

## Usage

The scope module is used during the visitor's traversal of the AST to keep track of function and variable definitions. This ensures that the definitions are available when needed, such as during semantic analysis or code generation.

```c
// Example usage
scope_T *scope = init_scope();
scope_stack_T *stack = init_scope_stack();
stack = push_scope_to_stack(stack, scope);

// Add function and variable definitions
scope_add_function_definition(scope, function_def);
scope_add_variable_definition(scope, variable_def);

// Retrieve definitions
AST_T *func = scope_get_function_definition(scope, "function_name");
AST_T *var = scope_get_variable_definition(scope, "variable_name");

// Clean up
stack = pop_scope_from_stack(stack);
free_scope_stack(stack);
```

This module is essential for managing the scope of definitions and ensuring that they are correctly handled during the visitor's traversal of the AST.
