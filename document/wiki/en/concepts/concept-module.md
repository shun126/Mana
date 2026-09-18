# Module

A `module` groups Actions and member definitions that you want to reuse in several Actors.

In Mana, rather than writing similar Actions over and over in each Actor, you can define the shared part as a Module and `extend` it from the Actors that need it.

```mana
module CommonActions
{
    action greet
    {
        print("Hello\n");
    }
}

actor Villager
{
    extend CommonActions;
}
```

In this example, `Villager` takes in the definitions of `CommonActions`.

## What a Module is for

A Module is not itself a unit of execution.

Unlike an `actor`, it is not instantiated when the VM starts and does not run Actions by itself; it is a unit of reuse that gives Actors shared features.

Conceptually, you can think of it like this:

```text
module CommonActions
        |
        | extend
        v
actor Villager

module CommonActions
        |
        | extend
        v
actor Guard
```

One Module can be used by several Actors.

## Think "reusing parts" rather than inheritance

The name `extend` may make you think of class inheritance in C++ or Java.

But rather than a mechanism for building class hierarchies, it is more accurate to understand a Mana Module as **a part that adds shared definitions to an Actor**.

For example, you can group these into Modules:

- Actions for conversation
- Shared reactions
- Shared waiting routines
- Behaviour shared by several kinds of NPC

## It combines with namespace

A Module can also be defined inside a namespace.

```mana
namespace Game::NPC
{
    module Talkable
    {
        action talk
        {
            print("Hello\n");
        }
    }
}
```

You can use it by its fully qualified name.

```mana
actor Villager
{
    extend Game::NPC::Talkable;
}
```

If a namespace is made searchable with `using`, you can also refer to it by its short name.

```mana
using Game::NPC;

actor Villager
{
    extend Talkable;
}
```

The current compiler resolves Module names through namespaces and `using` like this.

## How Actor and Module differ

| | Actor | Module |
| --- | --- | --- |
| Is a unit of execution | Yes | No |
| Created as an Actor when the VM starts | Yes | No |
| Can define Actions | Yes | Yes |
| Is the one that gets `extend`ed | Not normally | Yes |
| Main purpose | An independent unit of execution | Reusing shared definitions |

## When to use a Module

A Module is useful when several Actors have behaviour that means the same thing.

On the other hand, if code merely looks a little similar, you don't necessarily need to split it into a Module.

It helps to judge by asking: "is this behaviour one role shared by several Actors?"

## Definitions with the same name

If both the Actor itself and a Module define an Action or member with the same name, the compiler's symbol resolution rules apply.

We recommend not designing around overriding or precedence between same-named definitions, and instead choosing names that don't clash for each role you reuse.

For the syntax and constraints guaranteed by the current specification, see the [Module reference](../reference/reference-module.md).

## Summary

- A Module is a unit of reuse that gives Actors shared features
- A Module is not itself a unit of execution
- An Actor uses it with `extend ModuleName;`
- It is easier to understand as a "part" added to an Actor than as class inheritance
- It can be organised together with namespace / `using`

Next, we explain the **Phantom**, for creating Actors from a definition at run time.
