# **BLUNT**

## Overview

MyLanguage is a simple programming language designed for educational purposes. It supports variable definitions, function definitions, function calls, and basic arithmetic expressions.

# **_Syntax_**

### Variable Definitions

Variables can be defined using the roll keyword followed by the number of instances, the variable name, and the value. The with keyword is used to assign the value.

Example:

```
roll 1 joint with "sto tutto fatto";
roll 1 puro with "madonna";
```

### Function Definitions

Functions (referred to as "blunts") are defined using the blunt keyword followed by the function name and parameters. The function body is enclosed in curly braces {}.

Example:

```
blunt smokeWeedEveryDay()
{
roll weed with 19;
print("SMOKE WEED EVERY DAY!!!\n");
}
```

### Function Calls

Functions are called by their name followed by parentheses (). Arguments can be passed within the parentheses.

Example:

```
smokeWeedEveryDay();
```

### Return Statements

Functions can return values using the smoke keyword followed by the value to be returned.

Example:

```
blunt returnAString()
{
roll returnValue with "This should be the last line printed\n";
smoke returnValue;
}
```

## Built-in Functions

- **print**: Prints the provided arguments to the console.

Example:

```
print(joint, puro, "\n");
```

### Arithmetic Expressions

MyLanguage supports basic arithmetic operations such as addition +, subtraction -, multiplication \*, and division /. Parentheses () can be used to group expressions.

Example:

```
roll 1 result1 with 31 + 27 * (33-1);
```

# _First interaction with the language_

## Variable definitions

```
roll 1 joint with "sto tutto fatto";
roll 1 puro with "madonna";
```

## Function call

```
print(joint, puro, "\n");Functions and Scopes
```

## Functions (/ blunts) definition and scopes tests

```
roll 1 globalVar with "This is a global var";blunt smokeWeedEveryDay()
{
roll weed with 19;
print("SMOKE WEED EVERY DAY!!!\n");
}
```

## Blunts with arguments

```
blunt printTheString(theString)
{
print(theString);
}blunt printTwoString(string1, string2)
{
printTheString(string1);
printTheString(string2);
}
```

## Blunts with return (smoked) value

```
blunt returnAString()
{
roll returnValue with "This should be the last line printed\n";roll midLine with "Smoked";blunt returnALot()
{
roll 2 sushi with "a lot";blunt insideJoke()
{
roll lastScopeVar with "and never give up!\n";
print(midLine, sushi, lastScopeVar);
}insideJoke();smoke sushi;
}roll result with returnALot();


printTwoString(midLine, returnALot());

print("Stoned", result, "\n");

smoke returnValue;
}

smokeWeedEveryDay();

roll 1 result with returnAString();
print(result);
```

## Expressions evaluation tests

```
roll 1 result1 with 31 + 27 \* (33-1);

print("result1 is :", result1, "\n");

roll result2 with (42 + result1) - (123*44-2*2) / 4;

print("result 2 is:", result2, "\n");
```
