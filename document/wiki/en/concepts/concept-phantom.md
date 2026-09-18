# Phantom

A `phantom` is **a template for an Actor definition**, for creating Actors at run time when they are needed.

Its syntax is very similar to an Actor's.

```mana
phantom EnemyTemplate
{
    action appear
    {
        print("Enemy appeared\n");
    }
}
```

However, `actor` and `phantom` are handled differently when loaded into the VM.

## Actors are created at startup

An ordinary `actor` is instantiated when the Program Image is loaded into the VM, and registered in the VM's list of Actors.

```mana
actor Guide
{
    action main
    {
    }
}
```

`Guide` is treated as a unit of execution that lives for the whole program.

## Phantoms are not created at startup

A `phantom` does not create an Actor instance when the VM starts.

The VM keeps the Phantom's definition and can create an Actor from it when the C++ side needs one.

Conceptually, the flow is:

```text
Mana source
    |
    | phantom EnemyTemplate
    v
Program Image
    |
    v
Mana VM
    |
    | keeps only the definition
    |
    | C++: CreateActorFromPhantom(...)
    v
Run-time Actor
```

## Creating it from C++

The current VM has an API for creating an Actor from a Phantom.

```cpp
auto enemy = vm->CreateActorFromPhantom("EnemyTemplate", "Enemy01");
```

The first argument is the Phantom's definition name, and the second is the name of the Actor to create.

This can be developed into uses such as creating several run-time Actors from the same definition.

## When to use it

Think of things created when the game needs them, for example:

- Enemy characters
- Temporary event Actors
- Gimmicks placed dynamically
- Spawned NPCs

It suits units of execution that don't need to exist from startup, defined in advance on the Mana side.

## How Actor and Phantom differ

| | Actor | Phantom |
| --- | --- | --- |
| Definition can have Actions | Yes | Yes |
| Created automatically when the VM loads | Yes | No |
| Runs at normal startup | Yes | No |
| Created from C++ when needed | Not required | Its main use |
| Main purpose | A resident unit of execution | A template for dynamic creation |

## What about `init` / `main`?

Just defining a Phantom does not create an instance, so `init` / `main` do not run right after loading as they do for an ordinary Actor.

The detailed lifecycle after creation and how to use the C++ API are covered in Integration.

## Creating one directly from a script

The current specification defines no syntax for instantiating a `phantom` directly from a Mana script.

Creating Phantoms is the job of the VM API on the C++ side.

This shows that Phantom is a feature close to the boundary between Mana and the game engine.

## How it differs from Module

Module and Phantom are both different from an Actor itself, but their purposes differ greatly.

```text
Module
  -> Reuses shared definitions in existing Actors

Phantom
  -> A definition for creating new Actors at run time
```

It is easy to tell them apart if you think of a Module as "a feature part" and a Phantom as "a template for creation".

## Summary

- A Phantom is a template for an Actor definition
- It is not instantiated automatically when the VM loads
- It can be created from C++ with `CreateActorFromPhantom()`
- It can be used for dynamic enemies, NPCs, gimmicks and so on
- The current specification has no syntax for creating one directly from a script

Next, we sort out the ideas behind **Namespace**, which organises names in large Mana programs.
