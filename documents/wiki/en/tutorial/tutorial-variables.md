# Remembering state with variables

You talk to the guide twice and print how many times you have talked. A **variable** stores the value.

## Run it and see

This is the **whole file**. Replace `lesson.mn` in the Mana folder with it, save, and run it with `mana lesson.mn`.

```mana
actor Event
{
    action main
    {
        awaitCompletion(10, Guide->talk);
        awaitCompletion(10, Guide->talk);
    }
}

actor Guide
{
    int mTalkCount;

    action init
    {
        mTalkCount = 0;
    }

    action talk
    {
        mTalkCount = mTalkCount + 1;
        print("Talk count: %d\n", mTalkCount);
    }
}
```

**Expected output:**

```text
Talk count: 1
Talk count: 2
```

You can also run the [finished code that comes with Mana](../../../../examples/tutorial/05-variables.mn) with `mana examples/tutorial/05-variables.mn`.

## Type, name and value

`int mTalkCount;` declares a variable that holds an integer. `int` is the **type**, the kind of value it holds, and `mTalkCount` is the variable's name. The `init` Action sets its first value to 0.

`mTalkCount = mTalkCount + 1;` is an **assignment** that adds 1 to the current value and stores the result. Unlike an equation in maths, it computes the value on the right and puts it into the left. The first time it goes from 0 to 1, the second time from 1 to 2.

## Printing a value

In `print("Talk count: %d\n", mTalkCount);`, `%d` marks where the integer passed after it is printed. `\n` is a line break.

Printing a variable shows how far the work has got and how the value has changed.

## Keeping a value across Actions

This variable is declared inside `Guide` and outside its Actions, so it is an **Actor variable**. The Actions belonging to `Guide` can refer to it directly, while other Actors cannot modify it directly.

Starting the name with `m` is a convention that makes it easy to tell it is an Actor member; the language does not require it.

The variable keeps its value after the conversation's Action ends. But if you quit the program and start it again, it starts from 0 again. Nothing has been saved to a file.

Because the state belongs to `Guide`, it stays encapsulated with the Actions that use it. For more details, see the [variables reference](../reference/reference-variables.md).

## Local variables used inside one piece of work

A variable declared inside an Action is a **local variable** used by that work.

**Example that replaces the body of talk:**

```mana
int count = 0;
count = count + 1;
print("Talk count: %d\n", count);
```

Here each conversation starts from `count = 0`, so both times it prints 1.

## Common types

The following are **examples of declarations written inside an Action**.

```mana
int count = 3;
float distance = 1.5;
bool hasKey = true;
string message = "Welcome!";
```

| Type | Value it stores |
| --- | --- |
| `int` | An integer |
| `float` | A number that can have a fractional part |
| `bool` | One of two values, `true` or `false` |
| `string` | A string of text |

## Change one thing

Go back to the finished code and add a line to `Event` that requests a third conversation. If it prints up to `Talk count: 3`, you have confirmed that the state is kept.

## Read next

In [Changing behaviour with conditions](./tutorial-conditions.md), you branch on a value.
