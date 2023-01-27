# Cobalus Interpreter
Cobalus is my first toy programming language. It's a very simple language that supports loops(while and for), function, and variables.

It currently has just two basic types (list is not one of them) double and strings. 

Has support for code blocks and has variable shadowing(and yeah, this was definitely a PITA to implement).

Some examples of codes would be:

```
#WHILE LOOP
var a = 10; 

while(a >0 ) {
    # Print is a built-in function
    print(a);
    # No there is no --a
    a = a - 1;
}

#FOR LOOP
for (var i = 0; i < 10; i = i + 1 )
{
    # It supports if statements
    if (i == 8) {
        # Breaks too :)
        break;
    } else {
        print(i);
    }
}

# FUNCTIONS
func add(a, b)
{
    # Even with return, recursion is a little buggy.
    # So I do not recommend using it for now :(
    return a + b;
}

print(add(10, 5));

var end = "That's all folks";

print(end);

```

As you can see, this programming language is dynamic, and you do not need to specify the type of the variable. Since all numbers are treated as doubles is not possible to do '"Bank account: " + 4', as it would print something like:

'Bank account: 4.000000'

But it's possible to do comparasion between strings and is possible to concatenate them.

## Compilation
```
# Clang
clang++ -O3 -std=c++20 cobalus.cpp -o cobalus

# GCC
g++ -O3 -std=c++20 cobalus.cpp -o cobalus
```

### END

It's definitely not a production language, but it was good training. And yes, I didn't know about the visitor pattern until the code was almost complete. But I will try to bring a new language with more functions and a lot less bugs("Captain Hook never breaks a promise").
