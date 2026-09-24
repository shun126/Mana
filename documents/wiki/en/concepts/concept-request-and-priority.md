# Request and Priority

In Mana, you use a Request when you want an Actor to run an Action.

```mana
request(3, Guard->move);
```

This expression asks the `Guard` Actor to run its `move` Action at Priority 3.

## A Request is not a function call

An ordinary function call waits on the spot until the called work finishes.

A Request is different: it registers the Action in the target Actor's execution state.

```text
EventController
      │
      │ request(3, Guard->move)
      ▼
    Guard
      │
      └─ priority 3 : move
```

With a plain `request`, the side that issued the Request does not wait for the target Action to finish.

When you need to wait for the target Action to start or finish, use `awaitStart` or `awaitCompletion`.

## Priority expresses how important an Action is

In Mana, the larger the number, the higher the Priority.

For example, suppose Guard is running `patrol` at Priority 1, and `damage` is Requested at Priority 5.

```text
priority 5 : damage   ← runs
priority 1 : patrol   ← suspended
```

The higher-Priority Action runs first, and the original Action is suspended, keeping its state partway through.

When `damage` ends, `patrol` can go back to the saved position and carry on.

## A lower-Priority Request

A Request with a lower Priority than the Action currently running does not run right away.

```text
Current
priority 5 : battle

New Request
priority 2 : talk
```

Here `talk` is kept as the candidate to run at Priority 2, and becomes ready to run after the Priority 5 work has finished.

So Priority is not just a number for sorting; it is the mechanism that decides **which Action runs now and which Action waits, within an Actor**.

## Only one per Priority

In the current Mana implementation, one Actor cannot hold several Requests at the same Priority.

If a Request at Priority 3 is already registered and you Request a different Action at Priority 3, the new Request is not accepted.

```text
Guard
priority 3 : talk

request(3, Guard->move)
        ↓
Not accepted, because the same Priority is in use
```

So a Priority acts not only as "importance" but also as something like an execution slot inside the Actor.

Rather than giving Priorities very fine-grained numbers, it is easier to manage if the game decides on levels that mean something and uses those.

For example, you can decide on uses like these:

```text
1 : Normal behaviour
3 : Conversations and events
5 : Reacting to damage
8 : Forced cutscenes
```

What matters is not the numbers themselves but agreeing on what they mean within the project.

## When a Request is not accepted

A Request does not always succeed.

In the current implementation, a Request is not accepted in cases such as these:

- The Priority given is at or below the lowest Priority that can be used
- The Actor is halted
- The Actor is refusing Requests
- The same Priority is already in use
- The Action given does not exist

An ordinary Mana script has no syntax for receiving whether a Request succeeded as a return value. But to understand the execution model, it helps to remember that "a Request is a request; it does not always start a new Action".

## `sender`

In an Action that received a Request, `sender` refers to who sent that Request.

```mana
actor Guard
{
    action talk
    {
        if (sender == Guide)
        {
            print("Guide requested talk\n");
        }
    }
}
```

This lets the same Action behave differently depending on who Requested it.

## Requests and waiting

Choose among the Request instructions according to the purpose.

| Syntax | What the caller does |
| --- | --- |
| `request` | Carries on after asking |
| `awaitStart` | Waits until the Action at the given Priority has reached the point where it can run |
| `awaitCompletion` | Waits until the Action at the given Priority completes |
| `join` | Waits until the target Actor's Priority is the given value or lower |

`request` suits loosely coupling Actors, and `awaitCompletion` suits making the order of an event explicit. However, the await instructions carry on without waiting if the request is not accepted. Even after it is accepted, they wait on a condition about the target Actor's Priority, so check the exact release conditions in the [Request reference](../reference/reference-request.md).

## Priority is managed per Actor

Priority does not create one table of execution order for the whole game.

Each Actor manages the Requests it has received and its current Priority.

```text
Guide
priority 3 : talk

Guard
priority 5 : damage
priority 1 : patrol

Gate
priority 2 : open
```

Each Actor has its own execution state, and the VM advances them in turn.

So there is no simple global ranking such as "Guard's Priority 5 runs before Guide's Priority 3".

Priority decides **how Actions interrupt and are held back within the same Actor**.

## Why use Priority

In a game, some work should take precedence even if it means interrupting what is happening now.

For example:

```text
Walk
  ↓
Spot an enemy
  ↓
Fight
  ↓
Take damage
  ↓
Return to fighting
```

Expressing this with nothing but lots of state branches makes it complicated to manage returning to the original work.

In Mana, keeping the execution state per Priority gives a structure that can return to the Action that was interrupted.

## What to remember so far

- A Request asks an Actor to run an Action
- A plain `request` does not wait for it to finish
- The larger the Priority, the higher it is
- A higher Priority can interrupt the current Action
- A lower Priority is kept to run later
- The same Priority on the same Actor cannot hold several Requests
- Priority is managed per Actor
- The Requester can be referred to through `sender`

## Read next

Next, we put together the overall picture of how these Requests and Priorities run inside the Mana VM.

[Mana's execution model](./concept-execution-model.md)
