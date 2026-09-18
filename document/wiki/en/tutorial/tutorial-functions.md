# Grouping work into functions

You give a name to the calculation of how many more keys are needed, and use it by that name. Work that takes input and returns a result can be defined as a **function**.

## Run it and see

This is the **whole file**. Save it in the Mana folder as `lesson.mn` and run `mana lesson.mn` from the terminal you set up on the setup page. Replace the previous chapter's code with the whole file rather than adding to it.

```mana
int remainingKeys(int required, int owned)
{
    if (owned >= required)
    {
        return 0;
    }
    return required - owned;
}

actor Event
{
    action main
    {
        int missing = remainingKeys(3, 1);
        print("Missing keys: %d\n", missing);
    }
}
```

**Expected output:**

```text
Missing keys: 2
```

You can also run the [finished code that comes with Mana](../../../../examples/tutorial/08-functions.mn) from the Mana folder with this command.

```text
mana examples/tutorial/08-functions.mn
```


## Reading the input and the result

Broken down, `int remainingKeys(int required, int owned)` means the following.

| Part | Meaning |
| --- | --- |
| The first `int` | Returns an integer as its result |
| `remainingKeys` | The function's name |
| `int required` | Argument that receives the number needed |
| `int owned` | Argument that receives the number you have |

Calling `remainingKeys(3, 1)` passes 3 to `required` and 1 to `owned`. `return` gives back the result and ends that call. Here it returns 2, the result of `3 - 1`, which goes into `missing`.

If you have at least as many keys as needed, it reaches `return 0;` first, so the subtraction below never runs.

## Change one thing

Change the call to `remainingKeys(3, 5)`. The output is `Missing keys: 0`.

If you add this function to the earlier event, you can use `remainingKeys(3, 5) == 0` as the `if` condition and open the gate when you have enough keys.

## Functions and Actions have different jobs

A function does a calculation or similar inside the work that called it, and returns the result to the caller. An Action is something an Actor does, and it is what a Request targets.

| Purpose | How these lessons write it |
| --- | --- |
| Calculate how many keys are needed | `remainingKeys(3, 1)` |
| Ask the gate to open | `awaitCompletion(10, Gate->open)` |

## Functions that return nothing

**Example definition added outside the Actors:**

```mana
void printSeparator()
{
    print("-----\n");
}
```

`void` means it returns no result. Write `printSeparator();` inside an Action to call it.

`native` functions, implemented on the C++ side, are covered in the [integration guide](../integration/integration-native-functions.md). Declaring one in Mana does not create the matching processing in the game.
## Read next

Continue with [Interrupting and resuming with Priority](./tutorial-priority.md).
