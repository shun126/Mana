# Types

Mana is a statically typed language. Variables, arguments, return values and so on have a type.

## Basic types

The basic types the current Lexer recognises are:

| Type | Overview |
| --- | --- |
| `void` | Used for Functions and the like that return nothing |
| `char` | 8-bit integer type |
| `short` | 16-bit integer type |
| `bool` | Boolean value |
| `int` | 32-bit integer type |
| `float` | 32-bit floating-point type |
| `string` | String type |
| `pointer` | A type for low-level references |

Examples:

```mana
int count = 10;
float speed = 2.5;
bool opened = false;
string name = "Guard";
```

## `void`

`void` is not an ordinary variable type that holds a value; it mainly expresses "no return value".

```mana
void reset()
{
}
```

## `bool`

`bool` holds `true` or `false`.

```mana
bool enabled = true;
```

In conditions you can use the results of comparisons and logical operations.

## `string`

`string` holds a string of text.

```mana
string message = "Hello";
```

String literals are enclosed in `"`.

## The `actor` type

To pass a reference to an Actor as an argument of a Function or native Function, you can use the `actor` type.

```mana
void notify(actor target)
{
}
```

`self`, `sender` and the like are also treated as Actor references.

## Predefined compound types

The current compiler registers the following value types in advance, for common use.

| Type | Members |
| --- | --- |
| `vec2` | `float x`, `float y` |
| `vec3` | `float x`, `float y`, `float z` |
| `vec4` | `float x`, `float y`, `float z`, `float w` |
| `rotator` | `float pitch`, `float yaw`, `float roll` |
| `color` | `float r`, `float g`, `float b`, `float a` |

As with Structs, their members are referred to with `.`.

```mana
actor BuiltInTypeExample
{
    action main
    {
        vec3 position;
        position.x = 10.0;
        position.y = 20.0;
        position.z = 30.0;

        color tint;
        tint.r = 1.0;
        tint.g = 0.5;
        tint.b = 0.25;
        tint.a = 1.0;
    }
}
```

`transform` is not registered as a predefined type in the current compiler.

## Struct types

You can create user-defined types with `struct`.

```mana
struct Position
{
    float x;
    float y;
}

Position p;
```

Structs are covered in detail in [Struct](./reference-struct.md).

## `pointer`

`pointer` is a type used mainly for low-level purposes, such as the boundary between the VM and native Functions.

For ordinary game event scripting, we recommend using mainly `int`, `float`, `bool`, `string`, `actor`, the predefined compound types and Struct types.

## Type checking

The Mana Compiler checks that types agree in assignments, Function calls, return values, operations and so on.

Code whose types do not match is, as a rule, diagnosed at compile time.

## Related topics

- [Variables](./reference-variables.md)
- [Constants](./reference-constants.md)
- [Expressions](./reference-expressions.md)
- [Struct](./reference-struct.md)
