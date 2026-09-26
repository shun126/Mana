# Action

An `action` is a unit of work an Actor runs.

Unlike a Function, an Action is a Request target, and it is started, suspended and resumed according to Priority.

## Syntax

```mana
action actionName
{
    statements
}
```

Example:

```mana
actor NPC
{
    action talk
    {
        print("Hello\n");
    }
}
```

## Arguments and return values

In the current syntax, an Action has no arguments and no return value.

```mana
action talk
{
}
```

To share values between Actors, use the Actor's state, global data, Structs, Native Functions and so on, as suits the purpose.

## Running an Action

An Action can be run with `request`, `awaitStart`, `awaitCompletion` and so on.

```mana
request(1, NPC->talk);
```

`NPC->talk` is an Action reference.

## `->` and `::`

Action references use `->`.

```mana
NPC->talk
```

Namespace qualification uses `::`.

```mana
Game::NPC::Shopkeeper->talk
```

The old form `Actor::action` remains in the compiler as compatibility syntax, but it gives a deprecated warning. In new code, use `->`.

## `init` and `main`

`init` and `main` are Action names that get special treatment when the VM starts.

```mana
actor Example
{
    action init
    {
    }

    action main
    {
    }
}
```

The current VM Requests `init` at the highest Priority (2147483647) and `main` at Priority 0 for every Actor. It does not wait for every Actor's `init` to finish before starting `main`. Each Actor runs its queued Actions in Priority order after its own `init` finishes.

## Predefined values while an Action runs

Inside an Action you can use predefined values that describe the current execution state.

- `self` : the current Actor
- `sender` : the Actor that Requested this Action
- `priority` : the Priority currently running

Example:

```mana
actor NPC
{
    action talk
    {
        print("priority = %d\n", priority);
    }
}
```

`sender` holds the Request's sender, so it can be used to tell which Actor asked for the Action to run.

For details, see [Predefined symbols](./reference-predefined-symbols.md).

## Priority and suspension

When an Actor receives another Request while running an Action, Priority decides the order of execution.

- A higher Priority: interrupts the current Action
- A lower Priority: held back until the current Action ends
- The same Priority: in the current VM, if a Request at that Priority already exists, the new Request is not accepted

The larger the number, the higher the Priority.

For details, see [Request](./reference-request.md) and [Execution control](./reference-execution-control.md).

## How it differs from a Function

| Action | Function |
| --- | --- |
| A unit of execution of an Actor | An ordinary subroutine |
| A Request target | An ordinary function call |
| Has a Priority | Has no Priority |
| Can be suspended and resumed | Runs as part of the caller's work |
| No arguments or return value in the current syntax | Can have arguments and a return value |

## Related topics

- [Actor](./reference-actor.md)
- [Function](./reference-functions.md)
- [Request](./reference-request.md)
- [Execution control](./reference-execution-control.md)
- [Predefined symbols](./reference-predefined-symbols.md)
