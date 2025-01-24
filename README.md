# BLUNT

Yet another useless programming language, just for fun.

- [BLUNT](#blunt)
- [BASICS](#basics)
  - [Variables](#variables)
    - [Example](#example)
  - [Functions](#functions)
      - [Example](#example-1)
      - [Example](#example-2)
  - [Loops](#loops)
    - [Example](#example-3)
  - [Strings](#strings)
    - [Dot dot notation](#dot-dot-notation)
- [Other examples](#other-examples)
  - [Syntax Highlight](#syntax-highlight)


# BASICS

Here some useless basics in order to use ( why? ) blunt for code.

## Variables

Every variable could be managed as an array!
The syntax for instantiate a new variable is the following:

```(blunt)
roll [count] var_name with (=) 1;
```

Where `[count]` is an optional indicator for how many var_name you want to instantiate. This could be usefull for nothing!
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

**NOTE**: On runtime while snoopy values are the same it is represented with a single value. Only when a particular value changes it is converted in a real array. I.e. `snoopy.1 = "YAY";` converts the var snoopy from a string variable to an array prepresented as: `["HEY", "YAY", "HEY" x8]`

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

A **_blunt_** is a mix between a class and a method. It can hold both variable and blunts definitions in it. When a blunt is called, if no return value is specified, it returns itself. In this way it is possible to access the methods defined within it.

#### Example

```(blunt)
blunt classBlunt(initValue)
{
  # Every line in the class scope is part of the constructor

  keep initValue; # save initValue as a class attribute

  blunt printValue()
  {
    println(initValue);
  }

  println("class initialized");

}

# instance will contain the classBlunt definition
roll instance with classBlunt("hello");

# Access the printValue method of classBlunt
instance.printValue();
# "hello" 
```


It is also possible to define blunt classes inside a blunt. (Whoa!)
#### Example

```(blunt)
blunt class(initValue)
{
  keep initValue;

  blunt nestedClass(hiString)
  {
    keep hiString;

    blunt sayHi()
    {
      println(hiString);
    }
  }
}

smoke obj with class(10);
smoke nestedObj with obj.nestedClass("Hello mfs!");
nestedObj.sayHi();
# Hello mfs!
```

**NOTE**: _keeped_ attributes are treated as private fields. It is possible to access them only through methods.
## Loops

At the moment exists only one way to iterate over a loop. It is possible to iterate only through a variable (don't know why).
Using the `light` keyword it is possible to use a sort of for loop.

**_Keep in mind that it is not possible to light something without rolling something before_**

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

## Strings
I implemented few strings operation like concatenation or slicing.
Let's dive in:
```(blunt)
# Concatenation with +
roll str1 with "Smoke a";
roll str2 with "blunt";

println(str1 + " " + str2);
# Smoke a blunt

roll smokeablunt with "Smoke a blunt"

println(len(smokeablunt)) # built-in len()
# 14

roll smokea with smokeablunt...6; #from 1st to 5th chars
roll ablunt with smokeablunt.6..; #from 6th to last chars
roll abl with smokeablunt.6.10; #from 6th to 9th chars

println(smokea + ablunt);
# Smoke a blunt

println(abl);
# a bl

```
### Dot dot notation
You may noticed that there's a particular syntax for slices. It is what I called a **dot dot notation**, another useless feature of this useless language. The dot dot notation permits to take a slice of a string (or array) giving the first index and the last index. For example the line `"Hello".0.3` evaluates in `"Hel"`. Now if I want to use the last index I could write something like `"Hello".3.(len("Hello"))` (evaluates in `"lo"`) but is really ugly. For that reason I implemented the `..` symtax that on runtime will be the maximum (or minimum) index possible for a particular variable. So that `"Hello".3.(len("Hello"))` become `"Hello".3..`. Fantastic! 

**_NOTE_**: The `..` notation could be used also for the first index, in that case it stands for `.0` element. In combination it is possible to write `"Hello"....` to obtain the same string. (Another killer feature here ah!?)

# Other examples
You can see a bunch of examples in the `examples` folder.

## Syntax Highlight

In the `blunt-highlight` folder there is the extension for supporting `blunt`syntax highlight in VSCode. It can be installed by copying this folder into `.vscode/extensions` folder and refresh vscode
