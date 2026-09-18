# Module

A `module` groups Actions and member definitions reused by several Actors.

A Module is not itself a unit of execution. An `actor` uses the definitions a Module contains by `extend`ing it.

## Syntax

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

The basic form of `extend` is:

```mana
extend ModuleName;
```

## What a Module can contain

In the current grammar, a Module's body uses the same `actions` grammar as an Actor.

So a Module can contain definitions such as:

- Actions
- Member variables
- Constants
- `extend`

```mana
module Talkable
{
    int mTalkCount;

    const int kTalkPriority = 10;

    action talk
    {
        mTalkCount = mTalkCount + 1;
    }
}
```

## Modules inside a namespace

A Module can be defined inside a namespace.

```mana
namespace Game::NPC
{
    module Talkable
    {
        action talk
        {
        }
    }
}
```

It can be named by its fully qualified name.

```mana
actor Villager
{
    extend Game::NPC::Talkable;
}
```

If the namespace is added to the places searched with `using`, the short name also works.

```mana
using Game::NPC;

actor Villager
{
    extend Talkable;
}
```

## A Module is not an Actor

A Module is included in the Program Image as definition information, but it is not created as a unit of execution when the VM loads, the way an ordinary Actor is.

So it is not for sending a `request` to the Module itself and running it independently; it is used to add shared definitions to Actors.

## Name clashes

If a definition brought in by `extend` and a definition on the Actor side have the same name, the compiler's symbol resolution rules apply.

For Modules meant for reuse, we recommend not relying on overriding by same-named definitions, and using names that clearly separate the roles.

## Related topics

- [Actor](./reference-actor.md)
- [Action](./reference-action.md)
- [Namespace](./reference-namespace.md)
