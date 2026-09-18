# Execution control

This page gathers the statements that control waiting, suspending and resuming Actions, and whether Requests are accepted.

## `yield()`

```mana
yield();
```

Without ending the current Action, it suspends execution at that point and hands control back to the Mana VM.

The next time it gets a chance to run, it resumes after the `yield()`.

```mana
print("step 1\n");
yield();
print("step 2\n");
```

`yield()` is not a timed wait. It is not guaranteed to mean "wait one frame" or "wait one second", and when it runs again also depends on how the host updates the VM.

## `join`

```mana
join(priority, actorExpression);
```

Does not send a new Request; it waits for a target Actor that is already running.

```mana
join(0, NPC);
```

In the current VM, it waits until the target Actor's current Priority is the given Priority or lower.

Whereas `awaitCompletion` "sends a Request and waits for it to complete", `join` is an instruction for "waiting on an existing execution state".

## `rollback`

```mana
rollback priorityExpression;
```

Example:

```mana
rollback 1;
```

`rollback` ends the current Action's execution and winds the Priority execution states saved in the Actor back towards the given value.

The current VM's `Actor::Rollback` releases the current Priority, removes any execution states left above the given value as needed, and then restores a saved Action that can resume. If no Action can resume, the Actor goes back to the stopped state.

The VM also uses the same Rollback mechanism internally when an Action ends normally.

`rollback` has a large effect on control flow, so use it when you want to explicitly roll back Priority execution states, rather than for ordinary sequential work.

## `halt()`

```mana
halt();
```

Stops execution of the current Actor.

The current VM puts the Actor into the halt state and clears the interrupt execution states it holds. `Actor::Request` does not accept new Requests to a halted Actor.

Restarting the Actor on the VM side clears the halt state.

## `refuse()`

```mana
refuse();
```

Puts the Actor into the state of refusing Requests.

The current VM sets the Actor's `Refused` flag, and from then on does not accept new Requests that arrive.

It is not an instruction that clears execution states already registered.

## `comply()`

```mana
comply();
```

Clears the state of refusing Requests set by `refuse()`.

The current VM clears the `Refused` flag.

```mana
refuse();
// A section in which new Requests are refused
comply();
```

## `lock`

Syntax:

```mana
lock statement
```

It is normally combined with a block.

```mana
lock
{
    // statements
}
```

The current compiler generates a `NonPreEmptive` instruction at the start of a `lock` and a `PreEmptive` instruction at its end. In the current VM, these turn on and off the `Synchronized` flag in the current Priority execution state.

Importantly, the current `Actor::Request` implementation does not refer to this `Synchronized` flag directly when deciding whether to accept a Request or whether a Priority interrupts.

So don't treat `lock` in the current implementation as meaning the same as a C++ mutex or "an atomic section that is never interrupted". In this reference, it is treated as **syntax that, in the current implementation, switches a synchronised execution flag**.

## `request` / `awaitStart` / `awaitCompletion`

These instructions combine asking for an Action to run with waiting.

```mana
request(10, NPC->talk);
awaitStart(10, NPC->talk);
awaitCompletion(10, NPC->talk);
```

For details, see [Request](./reference-request.md).

## Getting the Priority

The Priority currently running is available through the predefined value `priority`.

```mana
print("%d\n", priority);
```

## Related topics

- [Action](./reference-action.md)
- [Request](./reference-request.md)
- [Actor](./reference-actor.md)
- [Mana's execution model](../concepts/concept-execution-model.md)
