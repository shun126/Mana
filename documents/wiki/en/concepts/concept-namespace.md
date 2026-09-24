# Namespace

A `namespace` organises the names of Actors, Modules, structs and so on logically, and keeps the same short name from clashing.

You learned how to write one in the Tutorial. This page sorts out the role Namespace plays in Mana.

## Files and namespaces are different things

Splitting into files is a way of organising source code physically.

A namespace is a way of organising the names in a program logically.

```text
File
  -> Where the code is written

namespace
  -> Which area the name belongs to
```

For example, code written in `npc.mn` does not automatically go into an `NPC` namespace.

## Giving names a meaningful hierarchy

```mana
namespace Game::Town
{
    actor Guard
    {
    }
}

namespace Game::Dungeon
{
    actor Guard
    {
    }
}
```

Both have the short name `Guard`, but their full names differ.

```text
Game::Town::Guard
Game::Dungeon::Guard
```

This way, you can safely use the same role name in different contexts.

## `::` shows where a name belongs

Mana uses `::` to qualify with a namespace.

```mana
request(10, Game::Town::Guard->talk);
```

`->`, on the other hand, expresses the relationship between an Actor and an Action.

```text
Game::Town::Guard -> talk
^^^^^^^^^^^^^^^^^    ^^^^
Actor's name         Action
```

The two have different roles.

## `using` helps with name lookup

Instead of writing the fully qualified name every time, you can add places to search with `using`.

```mana
using Game::Town;

actor EventController
{
    action main
    {
        request(10, Guard->talk);
    }
}
```

`using Game::Town;` makes `Game::Town` one of the places searched when an unqualified name is resolved.

You can also bring in a specific Actor or Module as a name.

```mana
using Game::Town::Guard;
```

In the current implementation, `using` can target a namespace or an Actor / Module.

## Names are resolved after parsing

The Mana compiler resolves names in the semantic analysis stage, after reading the source.

So code that defines a namespace after the `using`, like this, can still be resolved.

```mana
using Game::Town;

actor Controller
{
    action main
    {
        request(1, Guard->talk);
    }
}

namespace Game::Town
{
    actor Guard
    {
        action talk
        {
        }
    }
}
```

This works because the compiler does not simply fix names one line at a time from the top; it analyses meaning by looking at the whole compilation unit.

## The same namespace across several files

A namespace is not confined to one file.

By using the same logical namespace from several sources, you can split a large game by role.

```text
character.mn  -> Game::Character
npc.mn        -> Game::Character::NPC
enemy.mn      -> Game::Character::Enemy
event.mn      -> Game::Event
```

Making the file layout resemble the namespace layout can make things easier to follow, but the two don't have to be the same.

## Don't add too many `using` declarations

`using` can make code shorter, but if several namespaces have symbols with the same name, a name can become ambiguous.

In that case, make your intent explicit with the fully qualified name.

```mana
request(10, Game::Town::Guard->talk);
```

Prefer making it clear which Actor you refer to over keeping it short.

## A namespace is not a unit of execution

A namespace itself does not run on the VM, and it does not hold state the way an Actor does.

A namespace is purely **a compile-time mechanism for organising and resolving names**.

This is an important difference from Actors and Modules.

## Summary

- A namespace organises names logically
- It is a separate mechanism from splitting into files
- `::` qualifies with a namespace
- `->` refers to an Actor's Action
- `using` helps with name lookup
- Names are resolved during semantic analysis, so forward references are possible
- When a name is ambiguous, write the fully qualified name

Next, we sort out how Mana source runs, in terms of the relationship between the **Compiler and VM**.
