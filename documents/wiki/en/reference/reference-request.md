# Request

A Request is the mechanism for asking an Actor to run an Action.

In Mana, cooperation between Actors is not expressed with ordinary Function calls alone, but with Request and Priority.

## `request`

Syntax:

```mana
request(priority, actor_expression->actionName);
```

Example:

```mana
request(10, NPC->talk);
```

The first argument is the Priority, and the second is an Action reference.

The larger the value, the higher the Priority.

## Action references

The recommended syntax is `->`.

```mana
NPC->talk
```

With a namespace:

```mana
Game::NPC::Shopkeeper->talk
```

An expression of Actor type can also be used.

```mana
actor target;
request(1, target->talk);
```

The current Parser accepts `expression->actionName` as an Action reference.

The old form:

```mana
NPC::talk
```

is also recognised as compatibility syntax, but it gives a deprecated warning. Don't use it in new code.

## How `request` behaves

A plain `request` does not wait for the Action to complete.

```mana
request(10, NPC->talk);
print("continue\n");
```

The Actor that sent the Request carries on with the work that follows.

On the target Actor's side, it is handled according to Priority.

- The requested Priority is higher than the current one: it interrupts the current work
- The requested Priority is lower than the current one: it is kept to run later
- The requested Priority is the same as the current one: an execution state at that Priority already exists, so the new Request is not accepted

It is not a mechanism that queues several Actions at the same Priority.

## When a Request is not accepted

In the current VM's `Actor::Request`, a Request fails in at least these cases:

- The Priority is at or below the VM's lowest interrupt Priority
- The target Actor is halted
- The target Actor is in the `refuse()` state
- A Request at the same Priority already exists
- The Action given does not exist

The `request` statement in a script has no return value that reports success or failure.

## `awaitStart`

```mana
awaitStart(priority, actor_expression->actionName);
```

Sends a Request and makes the caller wait until the target Actor is in a state where it can start that Priority.

If the request is accepted, the wait ends when the target Actor's current Priority becomes the requested Priority or lower. It does not guarantee that the first statement of the requested Action has already run.

```mana
awaitStart(10, NPC->talk);
```

## `awaitCompletion`

```mana
awaitCompletion(priority, actor_expression->actionName);
```

Sends a Request and waits until the work at that Priority completes.

If the request is accepted, the wait ends when the target Actor's current Priority becomes lower than the requested Priority. It is not a mechanism that keeps a completion notice per request and waits for it.

```mana
awaitCompletion(10, NPC->talk);
```

## When an await request is not accepted

If the initial Request is not accepted, `awaitStart` and `awaitCompletion` carry on without waiting. They do not keep requesting until the same Priority is free. Returning from the wait does not by itself guarantee that the requested Action ran, or that its behaviour succeeded.

## Awaiting yourself

If `awaitStart` or `awaitCompletion` targets `self`, the current VM raises a script error.

```mana
awaitCompletion(10, self->talk); // error
```

This is because if an Actor waits on itself, the wait can only finish if the waiting Actor itself makes progress.

Sending a Request to yourself with a plain `request` is possible.

## `sender`

When a Request is accepted, the sending Actor is recorded as the `sender` of the target Action.

```mana
actor NPC
{
    action talk
    {
        // sender is the Actor that Requested this Action
    }
}
```

In some cases, such as the system Requests made when the VM starts, there is no sending Actor.

## How it differs from `join`

`join` does not send a new Request.

```mana
join(0, NPC);
```

It watches the Priority of the work the target Actor is already running, and waits until it is the given Priority or lower.

| Instruction | New Request | Waits |
| --- | --- | --- |
| `request` | Sends one | No |
| `awaitStart` | Sends one | Until it can start |
| `awaitCompletion` | Sends one | Until it completes |
| `join` | Doesn't send one | Until the existing Priority is the given value or lower |

## Related topics

- [Actor](./reference-actor.md)
- [Action](./reference-action.md)
- [Execution control](./reference-execution-control.md)
- [Request and Priority](../concepts/concept-request-and-priority.md)
