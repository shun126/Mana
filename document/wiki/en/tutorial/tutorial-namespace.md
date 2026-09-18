# Organising names with namespace

When there is a `Guide` in the town and another somewhere else, you need to tell the names apart. A **namespace** is a way of putting names into groups.

Replace the two files from the previous chapter with the following.

## Put the town's Actors into Town

**Whole file lesson/town.mn:**

```mana
namespace Town
{
    actor Guide
    {
        action talk
        {
            print("Guide: Welcome!\n");
        }
    }

    actor Gate
    {
        action open
        {
            print("Gate: Open.\n");
        }
    }
}
```

The full name of `Guide` is now `Town::Guide`, and `Gate` is `Town::Gate`. `::` separates the parts of a name that includes a namespace.

## Say which names you use

**Whole file lesson/main.mn:**

```mana
import "town.mn";

using Town;

actor Event
{
    action main
    {
        awaitCompletion(10, Guide->talk);
        awaitCompletion(10, Gate->open);
        print("Event: Finished.\n");
    }
}
```

With `using Town;`, you can refer to names inside `Town` in their short form. The `Guide` in this example means `Town::Guide`.

## Run it

Run it from the Mana folder.

```text
mana lesson/main.mn
```

**Expected output:**

```text
Guide: Welcome!
Gate: Open.
Event: Finished.
```

You can also use the [main.mn](../../../../examples/tutorial/12-namespace/main.mn) and [town.mn](../../../../examples/tutorial/12-namespace/town.mn) that come with Mana.

```text
mana examples/tutorial/12-namespace/main.mn
```

## Change one thing

Delete `using Town;` and replace the two requests in main with the following.

```mana
awaitCompletion(10, Town::Guide->talk);
awaitCompletion(10, Town::Gate->open);
```

If you get the same output, you are referring to them by their full names.

| Symbol | What it follows |
| --- | --- |
| `::` | A namespace. Example: `Town::Guide` |
| `->` | An Action of an Actor. Example: `Town::Guide->talk` |

If another namespace also has a `Guide` and several `using` declarations make it unclear which one is meant, give the full name.

## Files and namespaces organise different things

The file name `town.mn` alone does not create `Town`. The other way round, one namespace can be split across several files.

It is easiest to split files by role first, then add namespaces when names clash or you want to make clear where something belongs. For details, see the [Namespace reference](../reference/reference-namespace.md).

## From here to your own event

In the tutorial you started by printing text, then learned the order of the conversation and the gate, remembering with variables, conditions, loops, functions, interrupts, waiting, and organising files and names.

For a review, add the [conversation count](./tutorial-variables.md) to this town's `Guide` and use a [condition](./tutorial-conditions.md) to change what it says the first and second time. Have the controller request the conversation twice and check each output.

Where to go next depends on what you want to do.

- Understand how Actors advance: [Mana concepts](../concepts/README.md)
- Look up syntax and rules: [Language Reference](../reference/README.md)
- Connect to a real game: [integration guide](../integration/README.md)
