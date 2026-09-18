# Repeating work

You print 3, 2, 1 and then open the gate. `while` repeats work of the same shape.

## Run it and see

This is the **whole file**. Save it in the Mana folder as `lesson.mn` and run `mana lesson.mn` from the terminal you set up on the setup page. Replace the previous chapter's code with the whole file rather than adding to it.

```mana
actor Event
{
    action main
    {
        int remaining = 3;
        while (remaining > 0)
        {
            print("Remaining: %d\n", remaining);
            remaining = remaining - 1;
        }
        awaitCompletion(10, Gate->open);
    }
}

actor Gate
{
    action open
    {
        print("Gate: Open.\n");
    }
}
```

**Expected output:**

```text
Remaining: 3
Remaining: 2
Remaining: 1
Gate: Open.
```

You can also run the [finished code that comes with Mana](../../../../examples/tutorial/07-loops.mn) from the Mana folder with this command.

```text
mana examples/tutorial/07-loops.mn
```


## while checks its condition first, every time

`while (remaining > 0)` repeats the block as long as what remains is greater than 0. It subtracts 1 each time, so it eventually reaches 0 and leaves the loop.

| Value when the condition is checked | What happens |
| --- | --- |
| 3 | Print 3, change it to 2 |
| 2 | Print 2, change it to 1 |
| 1 | Print 1, change it to 0 |
| 0 | Skip the block and go on to opening the gate |

This is not a countdown in seconds. The loop itself has no way to wait for time to pass.

## Change one thing

If you set the initial `remaining` to 5, it prints from 5 down to 1. If you set it to 0, it opens the gate without printing a single number. Predict the result before you run it.

## for, when you know the count

**Example that replaces the whole main of the finished code:**

```mana
for (int i = 0; i < 3; i++)
{
    print("Step: %d\n", i);
}
awaitCompletion(10, Gate->open);
```

Inside the brackets of `for` come "what to do first; the condition for continuing; what to do after each round". `i++` increases the value by 1. Here it prints 0, 1 and 2, then opens the gate.

## If it never ends, stop it and investigate

If you delete `remaining = remaining - 1;` from the finished code, the condition never changes and the loop never ends. This is called an **infinite loop**. You don't need to try it, but if it happens by mistake, press Ctrl+C in the terminal to stop it.

Also be careful with a loop that just keeps waiting, assuming another Actor will change a variable eventually. In Mana this involves handing other work a chance to run. You learn `yield()` in [Waiting and synchronisation](./tutorial-wait-and-synchronization.md).

## Things to look up when you need them

`break` ends a loop partway through, and `continue` skips the rest of the current round. `do-while` runs the work at least once before checking the condition.

For details, including the dedicated `loop` syntax, go to the [statements reference](../reference/reference-statements.md). For now, being able to use `while` and `for` with an exit condition is enough.
## Read next

Continue with [Grouping work into functions](./tutorial-functions.md).
