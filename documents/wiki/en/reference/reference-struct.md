# Struct

A `struct` groups several values, and the Functions that work with them, into one type.

## Syntax

```mana
struct TypeName
{
    members
}
```

Example:

```mana
struct Status
{
    int hp;
    int mp;
}
```

It is used as a variable.

```mana
actor StatusExample
{
    action main
    {
        Status status;
        status.hp = 100;
        status.mp = 20;
    }
}
```

## Member variables

Variables can be declared inside a Struct.

```mana
struct CharacterData
{
    string name;
    int level;
    float speed;
    actor owner;
}
```

A Struct can also have another Struct as a member.

```mana
struct Position
{
    float x;
    float y;
}

struct Unit
{
    Position position;
    int hp;
}

actor UnitExample
{
    action main
    {
        Unit unit;
        unit.position.x = 10.0;
        unit.hp = 100;
    }
}
```

Members are referred to with `.`.

## Member Functions

Ordinary Functions can be defined inside a Struct.

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

In the current compiler, a Struct's member Function can contain ordinary Mana code too. For example, it can take an Actor as an argument and send it a Request.

```mana
struct Helper
{
    void call(actor target)
    {
        request(1, target->talk);
    }
}
```

## Native member Functions

`native` Functions can also be declared inside a Struct.

```mana
struct Transform
{
    native void reset();
}

actor TransformExample
{
    action main
    {
        Transform transform;
        transform.reset();
    }
}
```

The syntax on the calling side is the same as for an ordinary member Function.

For how it maps to the C++ side, see [Native Function](./reference-native.md) and [Native Functions Integration](../integration/integration-native-functions.md).

## How Struct and Actor differ

A Struct is a data type that groups values. Unlike an Actor, it does not become an independent unit that runs Actions.

| Struct | Actor |
| --- | --- |
| A data type | A unit of execution |
| Can have Functions | Can have Actions and state |
| Not a Request target | Is a Request target |
| Held as a variable | The VM manages the Actor instance |

## Related topics

- [Function](./reference-functions.md)
- [Actor](./reference-actor.md)
- [Native Function](./reference-native.md)
