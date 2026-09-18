# Source code structure

Mana source code is written in text files. The standard extension is `.mn`.

```text
hello.mn
```

## Comments

A single-line comment starts with `//`.

```mana
// A single-line comment
print("Hello\n");
```

A multi-line comment is enclosed in `/*` and `*/`.

```mana
/*
A comment
over several
lines
*/
```

In the current Lexer, nesting block comments is an error.

## Whitespace and line breaks

Spaces and tabs separate tokens. Line breaks are also not normally significant in the grammar.

A statement is usually ended with `;`.

```mana
int count = 0;
count = count + 1;
```

## Blocks

`{` and `}` group several declarations or statements.

```mana
if (count > 0)
{
    print("positive\n");
}
```

Actors, Actions, Functions, namespaces and so on also use blocks.

## Identifiers

In the current Lexer, an identifier can start with a letter, `_` or `?`, and can contain digits after that.

Examples:

```text
count
_count
Enemy01
?temporary
```

For readability, however, we recommend using mainly letters and `_` in ordinary code.

Reserved words cannot be used as identifiers.

## Integer literals

Decimal integers are written as they are.

```mana
0
10
1234
```

Hexadecimal integers start with `0x`.

```mana
0x10
0xFF
```

Binary integers start with `0b`.

```mana
0b1010
0b1111_0000
```

In the current Lexer, `_` can be used as a separator in binary notation.

## Floating-point literals

Floating-point numbers are written with a decimal point.

```mana
1.0
0.5
12.25
1.0e3
```

## String literals

A string is enclosed in `"`.

```mana
"Hello"
"Hello\n"
```

Escape sequences such as `\n` can be used inside a string.

## Boolean values

Boolean values are written with these reserved words.

```mana
true
false
```

## Nil

In the current implementation, the reserved word for an empty reference is `Nil`.

```mana
Nil
```

The `N` is upper case.

## Name qualification and Action references

`::` separates namespaces.

```mana
Game::AI::Enemy
```

`->` is used for Action references.

```mana
Enemy->think
```

The two have different roles.

- `::` : qualifies with a namespace
- `->` : refers to an Actor's Action

## Related topics

- [Types](./reference-types.md)
- [Expressions](./reference-expressions.md)
- [Operators](./reference-operators.md)
