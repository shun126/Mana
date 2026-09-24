# Thinking actor-oriented

Mana is a scripting language for **thinking of the many processes in a game as independent units of execution called Actors**.

Rather than packing NPCs, doors, event flow, enemy AI, effects and so on into one huge routine, you give each of them a role and have them cooperate by Requesting Actions when needed.

## What problem it solves

In a game, many processes appear to run at the same time.

For example:

- An NPC talks
- An enemy chases the player
- A door opens
- Event management remembers how far things have got
- An effect starts

If you gather all of these into one long routine, the needs of one process easily affect another, and managing state gets complicated.

In Mana, you split Actors by role.

```text
EventController
 ├─ Guide
 ├─ Gate
 └─ Guard
```

Each Actor has its own state and Actions.

## Actors are not only characters

The name Actor may suggest that it only represents game characters.

But an Actor in Mana is **the basic unit of execution that runs Actions independently**.

For example, all of these can be Actors:

- NPCs
- Enemies
- Gimmicks such as doors and switches
- Event flow management
- Scene control
- Conversation management
- Effect management

What matters is not whether it is a visible object, but **whether you want to give it its own responsibility and behaviour**.

## Actors cooperate through Requests

When an Actor wants another Actor to do something, in Mana it does not call the Action directly like a function; it Requests it.

```mana
request(3, Guard->move);
```

This way, the side that asks does not need to know the details of how Guard works inside.

```text
EventController
      │
      │ Request
      ▼
    Guard
      │
      └─ move Action
```

The central idea of Mana is to separate "who does what" into Actors and Actions, and to arrange "when it runs" with Requests and Priority.

## Independent, in shared time

The Mana VM advances several Actors in turn.

So it does not create an operating-system thread for each Actor. Each Actor has its own execution state and moves a little at a time as the VM advances, so from the game's point of view several processes can be treated as progressing in parallel.

This documentation describes this property as **cooperative pseudo-parallel execution**.

Keep it apart from real CPU parallelism or multithreading.

## How it relates to the general Actor Model

Mana is designed around Actors, but it is not a direct implementation of the academic Actor Model.

Mana has mechanisms of its own, for example:

- Action
- Request
- Priority
- Interrupting and resuming Actions
- Global variables
- Cooperative execution by the Mana VM

So rather than thinking of Mana's `actor` as "exactly the same as an actor in the general Actor Model", it is more accurate to think of it as **Mana's own Actor, for splitting game processing into independent units of execution**.

## When to make something an Actor

Splitting everything into Actors is not the goal.

Actors suit processing such as:

- It has its own state
- Other code asks it to do things
- It moves at different times from other processing
- You want to handle interruption by Priority
- It is easy to name as a responsibility in the game

Simple calculations and shared routines, on the other hand, are more natural as Functions.

```text
Actor / Action
    "Who does what" in the game

Function
    Calculations and shared routines used inside Actions
```

## Why think actor-oriented

Splitting responsibilities by Actor makes it easier to see game processing like this:

```text
The NPC talks
The Door opens
The Guard moves
The EventController Requests them
```

What sets Mana apart is that you can see code not as "a long list of instructions" but as **a structure in which several units of execution move the game forward by asking each other for work**.

## What to remember so far

- Mana splits processing around Actors
- Actors are not limited to characters
- An Actor has state and Actions
- Actors cooperate through Requests
- Mana's Actor is not identical to the academic Actor Model
- The VM advances several Actors cooperatively

## Read next

Next, we look closely at how the **Actions** an Actor has differ from ordinary Functions.

[The roles of Actor and Action](./concept-actor-and-action.md)
