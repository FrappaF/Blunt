- [BLUNT](#blunt)
- [INSTALLATION](#installation)
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

# BLUNT

Welcome to BLUNT, yet another useless programming language created just for fun. If you ever wanted to waste your time learning something completely unnecessary, you're in the right place!

# INSTALLATION

To install the compiler, simply run:
```sh
sudo make
```
This will create a `blunt.out` file, which is the actual interpreter of the language. For example, you can run:
```sh
./blunt.out examples/loops.blunt
```

If you want to use the `blunt` keyword, you can try running:
```sh
sudo make install-[linux | mac]
```
This will copy the executable to the `local/bin` folder, but it depends on your OS.

# BASICS

Here are some useless basics to help you use `blunt` for coding (but seriously, why?).

## Variables

Every variable can be managed as an array! The syntax for instantiating a new variable is as follows:
```blunt
roll [count] var_name with (=) 1;
```
The `[count]` parameter is optional and indicates how many instances of `var_name` you want to create. This is incredibly useful for absolutely nothing! For example, if you declare 10 variables named snoopy (`roll 10 snoopy with "HEY"`), you can access the i-th value of the i-th snoopy. Amazing, right?

### Example

```blunt
roll 10 snoopy with "HEY";

# Builtin println useful for printing
println(snoopy.3) # With the dot notation, I can access the fourth snoopy value
```

In this snippet, we've created 10 snoopies, each with the same value. Think of it as an array of values:
`["HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY", "HEY"]`

**NOTE**: At runtime, while all snoopy values are the same, they are represented by a single value. Only when a specific value changes does it convert into a real array. For example, `snoopy.1 = "YAY";` transforms the snoopy variable from a single string to an array represented as: `["HEY", "YAY", "HEY" x8]`

## Functions

The functions here are called **_blunts_**. The syntax is C-like:
```blunt
# Definition of a new blunt
blunt func_name(var1, var2, ...)
{
  # Body of the function

  # Instead of returning a result, we smoke that
  smoke result;
}
```

A **_blunt_** is an extraordinary hybrid between a class and a method. It can house both variable and blunt definitions within its majestic confines. When you summon a blunt, if you don't specify a return value, it graciously returns itself. This mind-blowing feature allows you to access the methods defined within it. Isn't that just the most incredible thing you've ever heard? (Spoiler: it's not.)

#### Example

```blunt
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

Hold onto your hats, because it gets even more "exciting"! You can define blunt classes inside another blunt. (Whoa! Mind-blowing, right? No, not really.)

#### Example

```blunt
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

**NOTE**: _kept_ attributes are treated as private fields. You can't just waltz in and access them directly. Oh no, you have to go through the proper channels, i.e., methods. It's like a secret club, but for variables. Exclusive access only!

## Loops

Prepare to be underwhelmed by the one and only way to iterate over a loop in `blunt`. Yes, you heard that right, there's just one way. And guess what? You can only iterate through a variable. Why? Who knows! But let's pretend it's the most amazing thing ever.

Using the `light` keyword, you can create a sort of for loop. It's like magic, but not really.

**_Keep in mind that it is not possible to light something without rolling something before_**. Yes, you need to roll before you light. It's the rule.

### Example

```blunt
roll 10 values with [0,1,2,3,4,5,6,7,8,9];

# Iterate using idx as index var as long as i < 8
light values [using idx < 8]
{
  values.idx = values.(idx+1);
}

# values = [1,2,3,4,5,6,7,8,9,9]
```

You don't actually need to specify the iterator variable and the iteration condition `[using i < 8]`. If you don't specify them, the compiler will create a default `i` iterator that keeps growing until it reaches the end of the list. So, you can simply write:

```blunt
light values
{
  values.i = values.i * 2;
}
```

## Strings

I've implemented a few string operations like concatenation and slicing. Let's dive into those "amazing" features:

```blunt
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

You may have noticed that there's a particular syntax for slices. It is what I called a **dot dot notation**, another useless feature of this useless language. The dot dot notation permits taking a slice of a string (or array) by giving the first index and the last index. 
For example, the line `"Hello".0.3` will evaluates to `"Hel"`. 
Now if I want to use the last index, I could write something like `"Hello".3.(len("Hello"))` (evaluates to `"lo"`) but that is really ugly. For that reason, I implemented the `..` syntax that at runtime will be the maximum (or minimum) index possible for a particular variable. So that `"Hello".3.(len("Hello"))` becomes `"Hello".3..`. Fantastic!

**_NOTE_**: The `..` notation can also be used for the first index, in which case it stands for `.0` element. In combination, it is possible to write `"Hello"....` to obtain the same string. (Another killer feature here, huh?)

# Other examples

You can see a bunch of examples in the `examples` folder.

## Syntax Highlight

In the `blunt-highlight` folder, there is an extension for supporting `blunt` syntax highlight in VSCode. It can be installed by copying this folder into the `.vscode/extensions` folder and refreshing VSCode.
