# Requesting an Action

The event controller `Event` asks the guide `Guide` to talk. The instruction for asking is `request`.

![How a request flows](../../../assets/en/diagrams/request-flow.svg)

## Run it and see

This is the **whole file**. Save it in the Mana folder as `lesson.mn` and run `mana lesson.mn` from the terminal you set up on the setup page. Replace the previous chapter's code with the whole file rather than adding to it.

```mana
actor Event
{
    action main
    {
        print("Event: Request.\n");
        request(10, Guide->talk);
    }
}

actor Guide
{
    action talk
    {
        print("Guide: Welcome!\n");
    }
}
```

**Expected output:**

```text
Event: Request.
Guide: Welcome!
```

You can also run the [finished code that comes with Mana](../../../../examples/tutorial/03-request.mn) from the Mana folder with this command.

```text
mana examples/tutorial/03-request.mn
```


## Choose who to ask and what to do

**Excerpt from the code above:**

```mana
request(10, Guide->talk);
```

The values passed to an instruction inside the brackets are called **arguments**. Several arguments are separated by `,`.

| Part | Meaning |
| --- | --- |
| `10` | The Priority. A larger number is higher |
| `Guide->talk` | The Action `talk` of `Guide` |
| `->` | Points from the Actor on the left to its Action on the right |

`10` is neither a number of seconds nor a repeat count. These lessons use `10` to begin with, and interrupting by priority comes in a later chapter.

This example compiles even though `Guide` is defined after `Event`. The compiler looks up names across the whole program.

## request does not wait for completion

`request` sends a request to the other Actor and carries on with the caller's work without waiting for it to finish. Moving on to the next line does not prove that the other Action has finished.

Also, if the same Priority is already in use or reserved on an Actor, a new request at that Priority is not accepted. **Writing two requests one after the other does not guarantee that the two jobs run in order.** The details come in [Priority](./tutorial-priority.md).

To open the gate after the conversation ends, you need an instruction that waits for completion. The next chapter uses it.

## Change one thing

Rename `talk` to `greet`. If you change both the definition `action talk` and the target `Guide->talk`, you get the same output.

If you change only one of them, the code refers to a name that does not exist. Read the compiler's diagnostics and make the names match.
## Read next

Continue with [Talk, then open the gate](./tutorial-small-event.md).
