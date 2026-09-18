# Constants

Use `const` for values that don't change.

## Syntax

```text
const type name = constant-expression;
```

Examples:

```mana
const int kMaxCount = 10;
const float kSpeed = 2.5;
const bool kDebug = false;
const string kMessage = "Hello";
```

## They cannot be assigned

A name declared with `const` cannot be assigned to later.

```mana
const int kMaxCount = 10;

// Error
kMaxCount = 20;
```

The Compiler diagnoses such assignments.

## The initial value is a constant expression

The initial value of a `const` must be an expression that can be evaluated at compile time.

```mana
const int kBase = 10;
const int kDouble = kBase * 2;
```

Things whose value is only decided at run time, such as Function calls, cannot be used as a constant's initial value.

`Nil` has a type of its own, and in the current compiler it cannot be used in a constant expression.

## Naming Priorities

Constants suit giving meaningful names to numbers such as Priorities.

```mana
const int kTalkPriority = 10;
const int kMovePriority = 5;
```

```mana
request(kTalkPriority, NPC->talk);
```

The purpose is easier to see than with a number written directly.

## Using them as array sizes

Integer constants can also be used as the size of a fixed-length array.

```mana
const int kValueCount = 4;

actor ArrayExample
{
    action main
    {
        int values[kValueCount];
    }
}
```

## About `define` / `undef`

The current Lexer still has the old-style `define` / `undef` tokens, but they are not part of the declaration syntax in the current `Parser.yy`.

So the new documentation does not treat `define` / `undef` as current syntax for declaring constants. In new code, use `const type name = value;`.

## Related topics

- [Types](./reference-types.md)
- [Variables](./reference-variables.md)
- [Expressions](./reference-expressions.md)
- [Predefined symbols](./reference-predefined-symbols.md)
