# The roles of Actor and Action

Mana thinks of Actors and Actions separately.

An Actor is **a unit of execution that holds state**, and an Action is **a behaviour that Actor can perform**.

```mana
actor Guard
{
    bool mAlert;

    action watch
    {
    }

    action move
    {
    }
}
```

In this example, `Guard` is the Actor, and `watch` and `move` are Actions.

## An Actor holds state

An Actor's member variables represent the state the Actor holds. Actions belonging to that Actor can refer to those variables directly.

```mana
actor Guide
{
    int mTalkCount;

    action init
    {
        mTalkCount = 0;
    }
}
```

The Actor itself remains after an Action ends, so the state can still be referred to when another Action runs next.

This lets you represent, for example:

- How many times someone has talked
- Whether a door is open
- Whether an NPC is on alert
- How far an event has progressed

## An Action is a behaviour others can request

An Action is what a Request targets.

```mana
request(3, Guard->move);
```

Here `Guard->move` is a reference to an Action.

Putting the Action inside the Actor makes it clear in the code whose responsibility that behaviour is.

```text
Guard
 ├─ watch
 ├─ move
 └─ talk
```

## How it differs from a Function

Actions and Functions can both group work, but their roles differ.

A Function is called normally from the current work, and returns to the caller when it ends.

```mana
int clampHp(int hp)
{
    if (hp < 0)
        return 0;
    return hp;
}
```

An Action, on the other hand, takes part in the Actor's execution model and is affected by Request and Priority.

```mana
request(5, Enemy->damage);
```

Roughly, they divide like this:

| Function | Action |
| --- | --- |
| Calculations and shared routines | A behaviour of an Actor |
| Called normally | Asked for with a Request |
| Returns to the caller | Can be interrupted or held back by Priority |
| Also usable outside Actors | Belongs to an Actor |

It helps to think of "calculate HP" as a Function and "the enemy takes damage" as an Action.

## Does only one Action exist at a time?

One Actor can have several Action definitions.

Also, at run time, Requests with different Priorities can arrive at the same Actor.

For example, with this state:

```text
priority 1 : patrol
priority 5 : damage
```

`damage` interrupts `patrol`.

After `damage` ends, `patrol` can go back to the execution position that was saved and carry on.

So an Action is not just "a member function of the Actor"; it is also **a unit of the Actor's execution state, including suspension and resumption**.

## Sharing an Actor's state between Actions

Actions can access the same Actor's member variables.

```mana
actor Door
{
    bool mOpened;

    action init
    {
        mOpened = false;
    }

    action open
    {
        if (mOpened)
            return;

        mOpened = true;
        print("Door opened\n");
    }
}
```

Keeping an Actor's state close to its Actions like this lets you describe "the thing that holds state" and "the behaviour that changes that state" as one unit.

## `init` and `main`

For an ordinary Actor, the VM automatically Requests `init` and `main` after loading the program.

`init` can be used to set up the initial state, and `main` to start the Actor's basic behaviour.

```mana
actor NPC
{
    int mState;

    action init
    {
        mState = 0;
    }

    action main
    {
        print("NPC started\n");
    }
}
```

However, not every Actor has to have both. You define only the Actions you need.

## Don't give an Action too much responsibility

If you pack the whole game's processing into one Action, you lose the benefit of splitting things into Actors.

For example, rather than doing all of this directly:

```text
EventController.main
    The NPC's conversation
    The Door's animation
    The Guard's movement
    Playing sound effects
    Saving state
```

splitting the responsibilities like this suits Mana's design better:

```text
EventController
    ├─ Request Guide->talk
    ├─ Request Gate->open
    └─ Request Guard->move
```

## How to think when designing Actors and Actions

First, put the responsibilities in the game into words.

```text
Who?          Does what?
Guide         talk
Gate          open
Guard         move
Event         start
```

Making "who" the Actor and "does what" the Action tends to give a natural structure.

## What to remember so far

- An Actor is a unit of execution that holds state
- An Action is a behaviour an Actor can perform
- An Action is what a Request targets
- A Function is ordinary processing; an Action takes part in the Actor's execution model
- An Action can be suspended and resumed because of Priority
- An Actor's member variables can be shared by several Actions

## Read next

The mechanism for having an Actor run an Action is the Request. Next, we sort out how it behaves together with Priority.

[Request and Priority](./concept-request-and-priority.md)
