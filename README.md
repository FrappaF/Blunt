# BLUNT

Yet another useless programming language, just for fun.

# BASICS

Here some useless basics in order to use ( why? ) blunt for code.

## Variables

Every variable could be managed as an array!
The syntax for instantiate a new variable is the following:

```(blunt)
roll [count] var_name with (=) 1;
```

Where `[count]` is an optional indicator for how many var_name you want to instantiate. This could be usefull for anything!
Let's say I have declared 10 variables named snoopy ( `roll 10 snoopy with "HEY"` ) now I can access the i-th value of the i-th snoopy.
Sounds great isn't it?

### Example

```(blunt)
roll 10 snoopy with "HEY";

# Builtin println useful for printing
println(snoopy.3) # With the dot annotation I can access at the fourth snoopy value
```

In this snippet we've created 10 snoopy with the same value each. I can treat this variable as an array of values:
`["HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY"]`

**NOTE**: On runtime while snoopy values are the same it is represented with a single value. Only when a particular variable value changes it is converted in a real array. I.e. `snoopy.1 = "YAY";` converts the var snoopy in an array prepresented as: `["HEY", "YAY", "HEY" x8]`

## Functions

The functions here are called **_blunts_**. The syntax is C like:

```(blunt)
# Definition of a new blunt
blunt func_name(var1, var2, ...)
{
  # Body of the function

  # Insead of returning a result we smoke that
  smoke result;
}
```

There is not yet any particular feature on that...

## Loops

At the moment exists only one way to iterate over a loop. It is possible to iterate only through a variable (don't know why).
Using the `light` keyword it is possible to use a sort of for loop.

**_Keep in mind that it is not possible to ligh something witouth rolling something before_**

### Example

```(blunt)
roll 10 values with [0,1,2,3,4,5,6,7,8,9];

# Iterate using idx as index var as long as i < 8
light values [using idx < 8]
{
  values.idx = values.(idx+1);
}

# values = [1,2,3,4,5,6,7,8,9,9]
```

It's not mandatory to indicate the iterator variable and the iteration condition `[using i < 8]`. If it's not specified the compiler will iinstantiate a default `i` iterator that keeps growing until it reaches the end of list, so that it is possible to write only:

```(blunt)
light values
{
  values.i = values.i * 2;
}
```

## Syntax Highlight

In the `blunt-highlight` folder there is the extension for supporting `blunt`syntax highlight in VSCode. It can be installed by copying this folder into `.vscode/extensions` folder and refresh vscode
