# Operators

This page lists the main operators available in Mana.

## Arithmetic operators

| Operator | Meaning |
| --- | --- |
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division |
| `%` | Remainder |
| `**` | Power |

Examples:

```mana
int a = 10 + 2;
int b = 10 % 3;
```

## Comparison operators

| Operator | Meaning |
| --- | --- |
| `==` | Equal |
| `!=` | Not equal |
| `<` | Less than |
| `<=` | Less than or equal |
| `>` | Greater than |
| `>=` | Greater than or equal |

## Logical operators

| Operator | Meaning |
| --- | --- |
| `&&` | Logical AND |
| `||` | Logical OR |
| `!` | Logical NOT |

```mana
if (enabled && visible)
{
}
```

## Bitwise operators

| Operator | Meaning |
| --- | --- |
| `&` | AND |
| `|` | OR |
| `^` | XOR |
| `~` | NOT |
| `<<` | Left shift |
| `>>` | Right shift |

## Assignment operators

```text
=
+=  -=  *=  /=  %=
&=  |=  ^=
<<= >>=
```

Examples:

```mana
count += 1;
flags |= 0x10;
```

## Increment / decrement

The current grammar accepts both the prefix and the postfix forms.

```mana
++count;
--count;
count++;
count--;
```

## The conditional operator

```mana
condition ? trueValue : falseValue
```

Example:

```mana
int sign = value >= 0 ? 1 : -1;
```

## `sizeof`

In the current grammar, `sizeof` takes a type in brackets.

```mana
sizeof(int)
sizeof(Position)
```

It is not syntax that accepts an arbitrary expression.

## The Action reference operator `->`

`->` is a Mana-specific operator for referring to an Actor's Action.

```mana
Enemy->think
```

```mana
request(10, Enemy->think);
```

Its meaning differs from pointer member access in C/C++.

## Namespace qualification `::`

`::` qualifies a name that includes a namespace.

```mana
Game::AI::Enemy
```

To go as far as an Action, combine the two.

```mana
Game::AI::Enemy->think
```

## Main precedence

For typical expressions other than assignment, the current Parser binds more tightly roughly in this order, from loosest to tightest:

```text
?:
&& ||
== !=
< <= > >=
| ^
&
<< >>
+ -
* / %
**
sizeof
! ~
Unary + -
++ --
```

Note in particular that `&&` and `||` are declared with the same precedence in the current Parser.

When you want to make your intent clear, use brackets rather than relying on precedence alone.

```mana
if ((a || b) && c)
{
}
```

## Related topics

- [Expressions](./reference-expressions.md)
- [Statements](./reference-statements.md)
