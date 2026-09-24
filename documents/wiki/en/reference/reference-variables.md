# Variables

A variable is a name for holding a value.

## Declaration

```mana
int count;
float speed;
bool opened;
```

You can give an initial value at the same time.

```mana
int count = 0;
float speed = 1.5;
bool opened = false;
```

## Assignment

```mana
count = 10;
speed = 2.0;
```

Compound assignment can also be used.

```mana
count += 1;
speed *= 2.0;
```

## Global variables

Declared outside any Actor or Function.

```mana
int gScore = 0;
```

Global variables are initialised before the `init` of ordinary Actors runs.

## Actor variables

There is syntax for declaring a variable inside an Actor, outside its Actions. However, with the implementation built from the current source on 2026-09-16, we have confirmed that referring to such a variable by name inside an Action, as below, fails to compile with `incomplete type name 'mOpened'`.

The following example shows the current limitation; it is not a finished example you can run as it is.

```mana
actor Door
{
    bool mOpened;

    action init
    {
        mOpened = false;
    }
}
```

The design is for this to keep an Actor's state between Actions, but for now the runnable lessons use [an example in which one Actor manages a global variable](../tutorial/tutorial-variables.md).

## Local variables

Declared inside the block of an Action or Function.

```mana
actor Counter
{
    action main
    {
        int count = 0;
        count = count + 1;
    }
}
```

A local variable is a temporary value used during that work.

## Struct members

```mana
struct Position
{
    float x;
    float y;
}

actor PositionExample
{
    action main
    {
        Position p;
        p.x = 10.0;
        p.y = 20.0;
    }
}
```

## Fixed-length arrays

Variable declarations can use fixed-length arrays.

```mana
actor ArrayExample
{
    action main
    {
        int values[4];
        values[0] = 10;
        values[1] = 20;
    }
}
```

The array size can be a positive integer literal or the name of an integer constant.

```mana
const int kValueCount = 4;

actor ArrayWithConstant
{
    action main
    {
        int values[kValueCount];
    }
}
```

A declarator can also have several `[]` in a row.

```mana
int grid[4][8];
```

Array elements are referred to with `[]`.

```mana
values[index]
```

If an index decided at run time is outside the array, the current VM stops that Actor with a `ScriptError` and does not continue the out-of-range access.

## `allocate` and `static`

At the top level, `allocate` and `static` can be used to lay out the VM's variable memory explicitly. These are lower-level features than ordinary game logic.

### `allocate`

```mana
allocate 1024
{
    int gReservedValue;
}
```

`allocate N { ... }` reserves an area of an explicit size in the global variable area and places the variables inside it. `N` is an integer literal giving the number of bytes.

### `static`

```mana
static
{
    float gStaticValue;
}
```

Variables in `static { ... }` are placed in the VM's static variable area, separate from ordinary global variables.

There is also a form that gives the size explicitly.

```mana
static allocate 512
{
    int gStaticReservedValue;
}
```

The current compiler checks that the variables declared in an explicit area fit in it.

Mana's `static` is syntax for choosing the static variable area inside the VM. Don't read it as having the same meaning or linkage rules as `static` in C++.

## Main scopes

| Where it is declared | Main use |
| --- | --- |
| Global | A value shared across the whole program |
| Inside an Actor | Per-Actor state |
| Inside an Action / Function | A temporary value |
| Inside a Struct | A member of the Struct |
| A `static` block | The VM's static variable area |

Use `const` for values that don't change.

## Related topics

- [Types](./reference-types.md)
- [Constants](./reference-constants.md)
- [Expressions](./reference-expressions.md)
- [Struct](./reference-struct.md)
- [Error Handling](../integration/integration-error-handling.md)
