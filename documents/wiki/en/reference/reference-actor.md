# Actor

An `actor` is Mana's basic unit of execution that runs Actions independently.

An Actor holds state, has several Actions, and can receive Requests from other Actors.

## Syntax

```mana
actor ActorName
{
    members
}
```

Example:

```mana
actor NPC
{
    int mTalkCount;

    action talk
    {
        mTalkCount++;
        print("Hello\n");
    }
}
```

## Actor members

Inside an Actor you mainly write:

- Actions
- Variables
- Constants
- Modules brought in with `extend`

Example:

```mana
actor Guard
{
    int mAlertLevel;
    const int kMaxAlert = 3;

    action patrol
    {
    }
}
```

An Actor's member variables are kept as the Actor's state, and their values remain after an Action ends.

## Creation when the VM starts

The Mana VM instantiates ordinary `actor`s when the Program Image is loaded.

This is the big difference from `phantom`. A Phantom is not instantiated at load time; it is created explicitly from the C++ side.

## `init` and `main`

After loading the program, the Mana VM requests special Actions from the Actors.

The current VM sends every Actor, in this order:

1. A Request for `init` at Priority 1
2. A Request for `main` at Priority 0

If an Actor does not define that Action, the Request does not run.

```mana
actor Example
{
    action init
    {
        print("init\n");
    }

    action main
    {
        print("main\n");
    }
}
```

`init` can be used to initialise state, and `main` for normal startup work.

## The actor type

`actor` can also be used as a type that holds a reference to an Actor.

```mana
actor target;
```

You can name an Action on an Actor reference.

```mana
request(1, target->talk);
```

For details on Action references, see [Request](./reference-request.md).

## Namespace

An Actor can be defined inside a namespace.

```mana
namespace Game::NPC
{
    actor Shopkeeper
    {
        action talk
        {
        }
    }
}
```

Its fully qualified name is `Game::NPC::Shopkeeper`.

## Actors are not limited to characters

An Actor is a unit of execution, not a concept only for game characters.

It can also be used for roles such as:

- Event flow
- UI control
- Gimmicks
- Scene management
- Battle flow
- Effect control

## Related topics

- [Action](./reference-action.md)
- [Request](./reference-request.md)
- [Execution control](./reference-execution-control.md)
- [Module](../concepts/concept-module.md)
- [Phantom](../concepts/concept-phantom.md)
