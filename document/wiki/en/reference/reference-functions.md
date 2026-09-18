# Function

A Function is an ordinary subroutine that takes values, groups some work, and returns a value when needed.

Unlike an Action, a Function is not a Request target; it runs synchronously within the flow of the code that called it.

## Syntax

```mana
return_type functionName(arguments)
{
    statements
}
```

Example:

```mana
int add(int a, int b)
{
    return a + b;
}
```

A Function can be called from an Action or from another Function.

```mana
actor FunctionExample
{
    action main
    {
        int value = add(2, 3);
        print("%d\n", value);
    }
}
```

## Arguments

Arguments are declared as a type followed by a name.

```mana
float distance(float x, float y)
{
    return x + y;
}
```

The type can be a built-in type, `actor` or a user-defined type.

## Return values

A Function with a return value returns it with `return expression;`.

```mana
int getCount()
{
    return 10;
}
```

If there is no return value, use `void`.

```mana
void reset()
{
    return;
}
```

Returning a value from a `void` Function, or writing a bare `return;` in a Function that returns a value, is a compile error.

## Struct member Functions

A Function can also be defined inside a `struct`.

```mana
struct Counter
{
    int value;

    void reset()
    {
        value = 0;
    }
}

actor CounterExample
{
    action main
    {
        Counter counter;
        counter.reset();
    }
}
```

It is called with `.`.

For Struct member Functions, see [Struct](./reference-struct.md).

## How it differs from an Action

| Function | Action |
| --- | --- |
| Runs through an ordinary function call | Can be run with a Request |
| Can have arguments | Has no arguments in the current syntax |
| Can have a return value | Has no return value in the current syntax |
| Part of the caller's work | A unit of execution of an Actor |

It helps to use Actions for independent behaviour in the game, and Functions for work reused inside Actions.

## Native Functions

A Function connected to the C++ side is declared with `native`.

```mana
native void playSound(string name);
```

For details, see [Native Function](./reference-native.md).

## Related topics

- [Struct](./reference-struct.md)
- [Action](./reference-action.md)
- [Native Function](./reference-native.md)
