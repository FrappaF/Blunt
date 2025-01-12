MyLanguage

Overview
MyLanguage is a simple programming language designed for educational purposes. It supports variable definitions, function definitions, function calls, and basic arithmetic expressions.

Syntax

Variable Definitions
Variables can be defined using the roll keyword followed by the number of instances, the variable name, and the value. The with keyword is used to assign the value.

Example:
roll 1 joint with "sto tutto fatto";
roll 1 puro with "madonna";

Function Definitions
Functions (referred to as "blunts") are defined using the blunt keyword followed by the function name and parameters. The function body is enclosed in curly braces {}.

Example:
blunt smokeWeedEveryDay()
{
    roll weed with 19;
    print("SMOKE WEED EVERY DAY!!!\n");
}

Function Calls
Functions are called by their name followed by parentheses (). Arguments can be passed within the parentheses.

Example:
smokeWeedEveryDay();

Return Statements
Functions can return values using the smoke keyword followed by the value to be returned.

Example:
blunt returnAString()
{
    roll returnValue with "This should be the last line printed\n";
    smoke returnValue;
}

Built-in Functions
- print: Prints the provided arguments to the console.

Example:
print(joint, puro, "\n");

Arithmetic Expressions
MyLanguage supports basic arithmetic operations such as addition +, subtraction -, multiplication *, and division /. Parentheses () can be used to group expressions.

Example:
roll 1 result1 with 31 + 27 * (33-1);

Examples

Hello World
## First interaction with the language

# Variable definitions 
roll 1 joint with "sto tutto fatto";
roll 1 puro with "madonna";

# Function call
print(joint, puro, "\n");

Functions and Scopes
# Functions (/ blunts) definition and scopes tests

roll 1 globalVar with "This is a global var";

blunt smokeWeedEveryDay()
{
    roll weed with 19;
    print("SMOKE WEED EVERY DAY!!!\n");
}

# Blunts with arguments
blunt printTheString(theString)
{
    print(theString);
}

blunt printTwoString(string1, string2)
{
    printTheString(string1);
    printTheString(string2);
}

# Blunts with return (smoked) value
blunt returnAString()
{
    roll returnValue with "This should be the last line printed\n";
    
    roll midLine with "Smoked";

    blunt returnALot()
    {
        roll 2 sushi with "a lot";

        blunt insideJoke()
        {
            roll lastScopeVar with "and never give up!\n";
            print(midLine, sushi, lastScopeVar);
        }

        insideJoke();

        smoke sushi;
    }

    roll result with returnALot();

    printTwoString(midLine, returnALot());

    print("Stoned", result, "\n");

    smoke returnValue;
}

smokeWeedEveryDay();

roll 1 result with returnAString();
print(result);

Expressions
# Expressions evaluation tests

roll 1 result1 with 31 + 27 * (33-1);

print("result1 is :", result1, "\n");

roll result2 with (42 + result1) - (123*44-2*2) / 4;

print("result 2 is:", result2, "\n");

If/Else Evaluation
# If/Else evaluation tests

blunt test(x, y)
{
    if(x > y)
    {
        smoke x;
    }
    elseif(x < y) {
        smoke y;
    } 
    else {
        smoke x;
    }
}

roll res with test(10, 11);
print("The biggest number between 10 and 11 is", res);

blunt areEquals(x, y)
{
    if(x == y)
    {
        smoke 1;
    }
    smoke 0;
}

roll resEq with areEquals(10, 10);
print("10 == 10?", resEq);

blunt complexTest(a, b, c)
{
    if(a > b and a > c)
    {
        smoke a;
    }
    elseif(b > a and b > c) {
        smoke b;
    } 
    elseif(not(a > b and a > c)) {
        smoke c;
    }
    else {
        smoke a + b + c;
    }
}

roll res1 with complexTest(5, 10, 15);
print("Expected: 15, Actual:", res1);

roll res2 with complexTest(20, 10, 5);
print("Expected: 20, Actual:", res2);

roll res3 with complexTest(10, 20, 10);
print("Expected: 20, Actual:", res3);

roll res4 with complexTest(10, 10, 10);
print("Expected: 10, Actual:", res4);

Variable with Multiple Values
# Variable with multiple value tests

roll single with "Weed";
print("Expected single value: \"Weed\"\nReal value:", single);

single = 10;
print("Expected new variable value: 10\nReal value:", single);

roll 3 multiple with [1, 2, 3];

single = multiple.0;
print("Expected first value: 1\nReal value:", single);

roll index with 1;
multiple.index = 5;
print("Array [1, 5, 3]:", multiple);

roll 3 same with 1;
print("Same multiple values [1, 1, 1]:", same.0, same.1, same.2);
