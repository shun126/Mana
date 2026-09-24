# Expressions

An expression is syntax that makes, refers to, calculates or assigns a value.

## Literals

```mana
10
1.5
true
"Hello"
Nil
```

## Variable references

```mana
count
speed
mOpened
```

## Arithmetic expressions

```mana
count + 1
speed * 2.0
(a + b) * c
```

## Comparison expressions

```mana
count == 0
count != 0
count < 10
count >= 1
```

The result of a comparison can be used in conditions and so on.

## Logical expressions

```mana
enabled && visible
ready || force
!finished
```

## Assignment expressions

```mana
count = 10
count += 1
speed *= 2.0
```

The target of an assignment is something writable, such as a variable or a member.

## The conditional operator

`?:` can be used.

```mana
int value = enabled ? 1 : 0;
```

## Function calls

```mana
calculate(10, 20)
```

A Struct's member Function is called with `.`.

```mana
value.reset()
```

## Member references

A Struct's members are accessed with `.`.

```mana
position.x
```

## Array elements

Array elements are referred to with `[]`.

```mana
values[index]
```

## Action references

An Actor's Action is referred to with `->`.

```mana
Enemy->think
Game::AI::Enemy->think
```

Action references are used with `request`, `awaitStart`, `awaitCompletion` and so on.

```mana
request(10, Enemy->think);
```

`::` qualifies a name with a namespace, and `->` is an Action reference.

## Predefined values

Mana has predefined symbols for referring to the execution situation.

```text
priority
self
sender
this
Nil
```

Where each one is valid and exactly what it means are covered in the "Predefined symbols" reference.

## `sizeof`

A `sizeof` operator is provided.

What it applies to and what it gives are covered in detail in the operators reference.

## Type checking

The Compiler checks that types agree in operations and assignments within expressions.

## Related topics

- [Types](./reference-types.md)
- [Variables](./reference-variables.md)
- [Operators](./reference-operators.md)
- [Statements](./reference-statements.md)
