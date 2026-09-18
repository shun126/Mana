# Namespace and using

A `namespace` organises the names of Actors, Modules, Structs, Functions, variables, constants and so on into a hierarchy.

## namespace

```mana
namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}
```

The parts of a fully qualified name are separated with `::`.

```mana
Game::AI::Enemy
```

`::` is the operator that qualifies a name with a namespace. Its role differs from `->`, used in Action references.

```mana
request(1, Game::AI::Enemy->think);
```

## Adding a namespace to the search with using

```mana
using Game::AI;

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}
```

`using Game::AI;` adds `Game::AI::Enemy` as a candidate when the unqualified name `Enemy` is resolved.

## using an Actor / Module

In the current implementation, `using` can resolve not only a namespace but also an Actor / Module.

```mana
namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}

using Game::AI::Enemy;
```

In this case the last name, `Enemy`, is added to the current scope as an alias.

In the current implementation, the symbols `using` can target are limited to Actors / Modules. It is not treated as syntax for `using` Structs, ordinary Functions and so on in the same way.

## Scope

`using` affects name resolution in the namespace scope its declaration belongs to.

When you leave the namespace, the `using` scope added inside it also ends.

## Forward references

Mana analyses all symbols and namespaces after parsing, so a `using` can refer to namespaces and Actors / Modules defined later.

```mana
using Game::AI;

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}

namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}
```

## Ambiguous names

If several candidates are found for the same unqualified name, the compiler reports an ambiguous reference as an error.

Typical diagnostics include:

- `ambiguous using`
- `ambiguous symbol reference`
- `ambiguous type reference`
- `ambiguous actor reference`
- `unresolved using`

When a name is ambiguous, use the fully qualified name.

## Files and namespaces are different concepts

Splitting source into files does not create namespaces automatically.

- File: a unit for organising source code physically
- namespace: a unit for organising names logically

You can combine several files into one Program Image and still avoid name clashes with namespaces.

## Related topics

- [Source files and import / include](./reference-source-files.md)
- [Module](./reference-module.md)
- [Request](./reference-request.md)
